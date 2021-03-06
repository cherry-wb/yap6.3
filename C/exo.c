/*************************************************************************
*									 *
*	 YAP Prolog 							 *
*									 *
*	Yap Prolog was developed at NCCUP - Universidade do Porto	 *
*									 *
* Copyright L.Damas, V.S.Costa and Universidade do Porto 1985-1997	 *
*									 *
**************************************************************************
*									 *
* File:		exo.c							 *
* comments:	Exo compilation						 *
*									 *
* Last rev:     $Date: 2008-07-22 23:34:44 $,$Author: vsc $		 *				 *
* $Log: not supported by cvs2svn $				 	 *
*                                                                        *
*									 *
*************************************************************************/

#include "Yap.h"
#include "clause.h"
#include "yapio.h"
#include "eval.h"
#include "tracer.h"
#ifdef YAPOR
#include "or.macros.h"
#endif	/* YAPOR */
#ifdef TABLING
#include "tab.macros.h"
#endif /* TABLING */
#if HAVE_STRING_H
#include <string.h>
#endif

//static int exo_write=FALSE;

//void do_write(void) { exo_write=TRUE;}

#define NEXTOP(V,TYPE)    ((yamop *)(&((V)->u.TYPE.next)))

#define MAX_ARITY 256

#define FNV32_PRIME 16777619
#define FNV64_PRIME ((UInt)1099511628211)

#define FNV32_OFFSET 2166136261 
#define FNV64_OFFSET ((UInt)14695981039346656037)


/* Simple hash function:
   first component is the base key.
   hash0 spreads extensions coming from different elements.
   spread over j quadrants.
 */
static BITS32
HASH(UInt arity, CELL *cl, UInt bnds[], UInt sz)
{
  UInt hash;
  UInt  j=0;

  hash = FNV32_OFFSET;
  while (j < arity) {
    if (bnds[j]) {
      unsigned char *i=(unsigned char*)(cl+j);
      unsigned char *m=(unsigned char*)(cl+(j+1));

      while (i < m) {
	hash = hash ^ i[0];
	hash = hash * FNV32_PRIME;
	i++;
      }
    }
    j++;
  }
  return hash;
}

static BITS32
NEXT(UInt hash)
{
  return (hash*997);
}

/* search for matching elements */
static int 
MATCH(CELL *clp, CELL *kvp, UInt arity, UInt bnds[])
{
  UInt j = 0;
  while (j< arity) {
    if ( bnds[j] && clp[j] != kvp[j])
      return FALSE;
    j++;
  }
  return TRUE;
}

static void
ADD_TO_TRY_CHAIN(CELL *kvp, CELL *cl, struct index_t *it)
{
  BITS32 old = (kvp-it->cls)/it->arity;
  BITS32 new = (cl-it->cls)/it->arity;
  BITS32 *links = it->links;
  BITS32 tmp = links[old]; /* points to the end of the chain */

  if (!tmp) {
    links[old] = links[new] = new;
  } else {
    links[new] = links[tmp];
    links[tmp] = new;
    links[old] = new;
  }
}

/* This is the critical routine, it builds the hash table *
 * each HT field stores a key pointer which is actually
 * a pointer to the point in the clause where one can find the element.
 *
 * The cls table indexes all elements that can be reached using that key.
 * 
 * Insert:
 * j = first
 * not match cij -> insert, open new chain
 * match ci..j ck..j -> find j = minarg(cij \= c2j), 
 * else j = +inf -> c2+ci
 * Lookup:
 * j= first
 * not match cij -> fail
 * match ci..j ck..j -> find j = minarg(cij \= c2j)
 * else
 */
static int
INSERT(CELL *cl, struct index_t *it, UInt arity, UInt base, UInt bnds[])
{
  CELL *kvp;
  BITS32 hash;
  int coll_count = 0;


  hash = HASH(arity, cl, bnds, it->hsize);
 next:
  kvp = EXO_OFFSET_TO_ADDRESS(it, it->key [hash % it->hsize]);
  if (kvp == NULL) {
    /* simple case, new entry */
    it->nentries++;
    it->key[hash % it->hsize ] = EXO_ADDRESS_TO_OFFSET(it, cl);
    return TRUE;
  } else if (MATCH(kvp, cl, arity, bnds))  {
    it->ntrys++;
    ADD_TO_TRY_CHAIN(kvp, cl, it);
    return TRUE;
  } else {
    coll_count++;
    if (coll_count == 32)
      return FALSE;
    it->ncollisions++;
    //  printf("#");
    hash =  NEXT(hash);
    //if (exo_write) printf("N=%ld\n", hash);
    goto next;
  }
}

static yamop *
LOOKUP(struct index_t *it, UInt arity, UInt j, UInt bnds[])
{
  CACHE_REGS
  CELL *kvp;
  BITS32 hash;

  /* j is the firs bound element */
  /* check if we match */
  hash = HASH(arity, XREGS+1, bnds, it->hsize);
 next:
  /* loop to insert element */
  kvp = EXO_OFFSET_TO_ADDRESS(it, it->key[hash % it->hsize]);
  if (kvp == NULL) {
    /* simple case, no element */
    return FAILCODE;
  } else if (MATCH(kvp, XREGS+1, arity, bnds))  {
    S = kvp;
    if (!it->is_key && it->links[(S-it->cls)/arity])
      return it->code;
    else
      return NEXTOP(NEXTOP(it->code,lp),lp);
  } else {
    /* collision */
    hash =  NEXT(hash);
    goto next;
  }
}

static int
fill_hash(UInt bmap, struct index_t *it, UInt bnds[])
{
  UInt i;
  UInt arity = it->arity;
  CELL *cl = it->cls;

  for (i=0; i < it->nels; i++) {
    if (!INSERT(cl, it, arity, 0, bnds))
      return FALSE;
    cl += arity;
  }
  for (i=0; i < it->hsize; i++) {
    if (it->key[i]) {
      UInt offset = it->key[i]/arity;
      UInt last = it->links[offset];
      if (last) {
      /* the chain used to point straight to the last, and the last back to the origibal first */
	it->links[offset] = it->links[last];
	it->links[last] = 0;
      }
    }
  }
  return TRUE;
}

static struct index_t *
add_index(struct index_t **ip, UInt bmap, PredEntry *ap, UInt count, UInt bnds[])
{
  CACHE_REGS
  UInt ncls = ap->cs.p_code.NOfClauses, j;
  CELL *base = NULL;
  struct index_t *i;
  size_t sz;
  yamop *ptr;
  
  sz =   (CELL)NEXTOP(NEXTOP((yamop*)NULL,lp),lp)+ap->ArityOfPE*(CELL)NEXTOP((yamop *)NULL,x) +(CELL)NEXTOP(NEXTOP((yamop *)NULL,p),l);
  if (!(i = (struct index_t *)Yap_AllocCodeSpace(sizeof(struct index_t)+sz))) {
    CACHE_REGS
    save_machine_regs();
    LOCAL_Error_Size = 3*ncls*sizeof(CELL);
    LOCAL_ErrorMessage = "not enough space to index";
    Yap_Error(OUT_OF_HEAP_ERROR, TermNil, LOCAL_ErrorMessage);
    return NULL;
  }
  i->is_key = FALSE;
  i->next = *ip;
  i->prev = NULL;
  i->nels = ncls;
  i->arity = ap->ArityOfPE;
  i->ap = ap;
  i->bmap = bmap;
  i->is_key = FALSE;
  i->hsize = 2*ncls;
  if (count) {
    if (!(base = (CELL *)Yap_AllocCodeSpace(sizeof(BITS32)*(ncls+i->hsize)))) {
      CACHE_REGS
      save_machine_regs();
      LOCAL_Error_Size = sizeof(CELL)*(ncls+i->hsize);
      LOCAL_ErrorMessage = "not enough space to generate indices";
      Yap_FreeCodeSpace((void *)i);
      Yap_Error(OUT_OF_HEAP_ERROR, TermNil, LOCAL_ErrorMessage);
      return NULL;
    }
    bzero(base, sizeof(CELL)*(ncls+i->hsize));
  }
  i->size = sizeof(CELL)*(ncls+i->hsize)+sz+sizeof(struct index_t);
  i->key = (CELL *)base;
  i->links = (CELL *)(base+i->hsize);
  i->ncollisions = i->nentries = i->ntrys = 0;
  i->cls = (CELL *)((ADDR)ap->cs.p_code.FirstClause+2*sizeof(struct index_t *)); 
  *ip = i;
  while (count) {
    if (!fill_hash(bmap, i, bnds)) {
      size_t sz;
      i->hsize += ncls;
      if (i->is_key) {
	sz = i->hsize*sizeof(BITS32);
      } else {
	sz = (ncls+i->hsize)*sizeof(BITS32);
      }
      if (base != realloc(base, sz))
	return FALSE;
      bzero(base, sz);
      i->key = (CELL *)base;
      i->links = (CELL *)(base+i->hsize);
      i->ncollisions = i->nentries = i->ntrys = 0;
      continue;
    }
    fprintf(stderr, "entries=%ld collisions=%ld trys=%ld\n", i->nentries, i->ncollisions, i->ntrys);
    if (!i->ntrys && !i->is_key) {
      i->is_key = TRUE;
      if (base != realloc(base, i->hsize*sizeof(BITS32)))
	return FALSE;
    }
    /* our hash table is just too large */
    if (( i->nentries+i->ncollisions  )*10 < i->hsize) {
      size_t sz;
      i->hsize = ( i->nentries+i->ncollisions  )*10;
      if (i->is_key) {
	sz = i->hsize*sizeof(BITS32);
      } else {
	sz = (ncls+i->hsize)*sizeof(BITS32);
      }
      if (base != realloc(base, sz))
	return FALSE;
      bzero(base, sz);
      i->key = (CELL *)base;
      i->links = (CELL *)(base+i->hsize);
      i->ncollisions = i->nentries = i->ntrys = 0;
    } else {
      break;
    }
  }
  ptr = (yamop *)(i+1);
  i->code = ptr;
  if (count)
    ptr->opc = Yap_opcode(_try_exo);
  else
    ptr->opc = Yap_opcode(_try_all_exo);
  ptr->u.lp.l = (yamop *)i;
  ptr->u.lp.p = ap;
  ptr = NEXTOP(ptr, lp);
  if (count)
    ptr->opc = Yap_opcode(_retry_exo);
  else
    ptr->opc = Yap_opcode(_retry_all_exo);
  ptr->u.lp.p = ap;
  ptr->u.lp.l = (yamop *)i;
  ptr = NEXTOP(ptr, lp);
  for (j = 0; j < i->arity; j++) {
    ptr->opc = Yap_opcode(_get_atom_exo);
#if PRECOMPUTE_REGADDRESS
    ptr->u.x.x = (CELL) (XREGS + (j+1));
#else
    ptr->u.x.x = j+1;
#endif
    ptr = NEXTOP(ptr, x);
  }
  ptr->opc = Yap_opcode(_procceed);
  ptr->u.p.p = ap;
  ptr = NEXTOP(ptr, p);
  ptr->opc = Yap_opcode(_Ystop);
  ptr->u.l.l = i->code;
  Yap_inform_profiler_of_clause((char *)(i->code), (char *)NEXTOP(ptr,l), ap, GPROF_INDEX); 
  return i;
}

yamop  *
Yap_ExoLookup(PredEntry *ap USES_REGS) 
{
  UInt arity = ap->ArityOfPE;
  UInt bmap = 0L, bit = 1, count = 0, j, j0 = 0;
  struct index_t **ip = (struct index_t **)(ap->cs.p_code.FirstClause);
  struct index_t *i = *ip;
  
  for (j=0; j< arity; j++, bit<<=1) {
    Term t = Deref(XREGS[j+1]);
    if (!IsVarTerm(t)) {
      bmap += bit;
      LOCAL_ibnds[j] = TRUE;
      if (!count) j0= j; 
      count++;
    } else {
      LOCAL_ibnds[j] = FALSE;
    }
    XREGS[j+1] = t;
  }

  while (i) {
    //    if (i->is_key && (i->bmap & bmap)  == i->bmap) {
    //  break;
    // }
    if (i->bmap == bmap) {
      break;
    }
    ip = &i->next;
    i = i->next;
  }
  if (!i) {
    i = add_index(ip, bmap, ap, count, LOCAL_ibnds);
  }
  if (count)
    return LOOKUP(i, arity, j0, LOCAL_ibnds);
  else
    return i->code;
}

CELL
Yap_NextExo(choiceptr cptr, struct index_t *it) 
{
  CACHE_REGS
  CELL offset = ADDRESS_TO_LINK(it,(CELL *)((CELL *)(B+1))[it->arity]);
  CELL next = it->links[offset];
  ((CELL *)(B+1))[it->arity] = (CELL)LINK_TO_ADDRESS(it, next);
  S = it->cls+it->arity*offset;
  return next;
}

static Int 
p_exodb_get_space( USES_REGS1 )
{				/* '$number_of_clauses'(Predicate,M,N) */
  Term            t = Deref(ARG1);
  Term            mod = Deref(ARG2);
  Term            tn = Deref(ARG3);
  UInt		  arity;
  Prop            pe;
  PredEntry      *ap;
  MegaClause *mcl;
  UInt ncls;
  UInt required;
  struct index_t **li;


  if (IsVarTerm(mod)  || !IsAtomTerm(mod)) {
    return(FALSE);
  }
  if (IsAtomTerm(t)) {
    Atom a = AtomOfTerm(t);
    arity = 0;
    pe = PredPropByAtom(a, mod);
  } else if (IsApplTerm(t)) {
    register Functor f = FunctorOfTerm(t);
    arity = ArityOfFunctor(f);
    pe = PredPropByFunc(f, mod);
  } else {
    return FALSE;
  }
  if (EndOfPAEntr(pe))
    return FALSE;
  ap = RepPredProp(pe);
  if (ap->PredFlags & (DynamicPredFlag|LogUpdatePredFlag
#ifdef TABLING
		       |TabledPredFlag
#endif /* TABLING */
		       )) {
    Yap_Error(PERMISSION_ERROR_MODIFY_STATIC_PROCEDURE,t,"dbload_get_space/4");
    return FALSE;
  }
  if (IsVarTerm(tn)  || !IsIntegerTerm(tn)) {
    return FALSE;
  }
  ncls = IntegerOfTerm(tn);
  if (ncls <= 1) {
    return FALSE;
  }

  required = ncls*arity*sizeof(CELL)+sizeof(MegaClause)+2*sizeof(struct index_t *);
  while (!(mcl = (MegaClause *)Yap_AllocCodeSpace(required))) {
    if (!Yap_growheap(FALSE, required, NULL)) {
      /* just fail, the system will keep on going */
      return FALSE;
    }
  }
  Yap_ClauseSpace += required;
  /* cool, it's our turn to do the conversion */
  mcl->ClFlags = MegaMask|ExoMask;
  mcl->ClSize = required;
  mcl->ClPred = ap;
  mcl->ClItemSize = arity*sizeof(CELL);
  mcl->ClNext = NULL;
  li = (struct index_t **)(mcl->ClCode);
  li[0] = li[1] = NULL;
  ap->cs.p_code.FirstClause =
    ap->cs.p_code.LastClause =
    mcl->ClCode;
  ap->PredFlags |= MegaClausePredFlag|SourcePredFlag;
  ap->cs.p_code.NOfClauses = ncls;
  if (ap->PredFlags & (SpiedPredFlag|CountPredFlag|ProfiledPredFlag)) {
    ap->OpcodeOfPred = Yap_opcode(_spy_pred);
  } else {
    ap->OpcodeOfPred = Yap_opcode(_enter_exo);
  }
  ap->CodeOfPred = ap->cs.p_code.TrueCodeOfPred = (yamop *)(&(ap->OpcodeOfPred)); 
  return Yap_unify(ARG4, MkIntegerTerm((Int)mcl));
}

#define DerefAndCheck(t, V)			\
  t = Deref(V); if(IsVarTerm(t) || !(IsAtomOrIntTerm(t))) Yap_Error(TYPE_ERROR_ATOM, t0, "load_db");

static int 
store_exo(yamop *pc, UInt arity, Term t0)
{
  Term t;
  CELL *tp = RepAppl(t0)+1,
    *cpc = (CELL *)pc;
  UInt i;
  for (i = 0; i< arity; i++) {
    DerefAndCheck(t, tp[0]);
    *cpc = t;
    tp++;
    cpc++;
  }
  return TRUE;
}

static Int 
p_exoassert( USES_REGS1 )
{				/* '$number_of_clauses'(Predicate,M,N) */
  Term            thandle = Deref(ARG2);
  Term            tn = Deref(ARG3);
  PredEntry       *pe;
  MegaClause      *mcl;
  Int              n;


  if (IsVarTerm(thandle)  || !IsIntegerTerm(thandle)) {
    return FALSE;
  }
  mcl = (MegaClause *)IntegerOfTerm(thandle);
  if (IsVarTerm(tn)  || !IsIntegerTerm(tn)) {
    return FALSE;
  }
  n = IntegerOfTerm(tn);
  pe = mcl->ClPred;
  return store_exo((yamop *)((ADDR)mcl->ClCode+2*sizeof(struct index_t *)+n*(mcl->ClItemSize)),pe->ArityOfPE, Deref(ARG1));
}

void 
Yap_InitExoPreds(void)
{
  CACHE_REGS
  Term cm = CurrentModule;

  CurrentModule = DBLOAD_MODULE;
  Yap_InitCPred("exo_db_get_space", 4, p_exodb_get_space, 0L);
  Yap_InitCPred("exoassert", 3, p_exoassert, 0L);
  CurrentModule = cm;
}
