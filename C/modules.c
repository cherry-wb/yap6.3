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
 File:		modules.c						 *
* Last rev:								 *
* mods:									 *
* comments:	module support						 *
*									 *
*************************************************************************/
#ifdef SCCS
static char     SccsId[] = "%W% %G%";
#endif

#include "Yap.h"
#include "Yatom.h"
#include "Heap.h"

STATIC_PROTO(Int p_current_module, (void));
STATIC_PROTO(Int p_current_module1, (void));
STD_PROTO(void InitModules, (void));

#define ByteAdr(X) ((char *) &(X))
Term 
Module_Name(CODEADDR cap)
{
  PredEntry      *ap = (PredEntry *)cap;

  if (!ap->ModuleOfPred)
    /* If the system predicate is a metacall I should return the
       module for the metacall, which I will suppose has to be
       reachable from the current module anyway.

       So I will return the current module in case the system
       predicate is a meta-call. Otherwise it will still work.
    */
    return(ModuleName[CurrentModule]);
  else
    return (ModuleName[ap->ModuleOfPred]);
}

int 
LookupModule(Term a)
{
  unsigned int             i;
	
  for (i = 0; i < NoOfModules; ++i)
    if (ModuleName[i] == a)
      return (i);
  ModuleName[i = NoOfModules++] = a;
  return (i);
}

static Int 
p_current_module(void)
{				/* $current_module(Old,New)		 */
  Term            t;
  unsigned int             i;
	
  if (!unify_constant(ARG1, ModuleName[CurrentModule]))
    return (0);
  t = Deref(ARG2);
  if (IsVarTerm(t) || !IsAtomTerm(t))
    return (0);
  for (i = 0; i < NoOfModules; ++i)
    if (ModuleName[i] == t) {
       *CurrentModulePtr = MkIntTerm(i);
      return (TRUE);
    }
    *CurrentModulePtr = MkIntTerm(NoOfModules);
  ModuleName[NoOfModules++] = t;
  return (TRUE);
}

static Int
p_current_module1(void)
{				/* $current_module(Old)		 */
  if (!unify_constant(ARG1, ModuleName[CurrentModule]))
    return (0);
  return (1);
}

static Int
p_change_module(void)
{				/* $change_module(New)		 */
  Term t = MkIntTerm(LookupModule(Deref(ARG1)));
  UpdateTimedVar(AbsAppl(CurrentModulePtr-1), t);
  return (TRUE);
}

void 
InitModules(void)
{
  ModuleName[PrimitivesModule = 0] =
    MkAtomTerm(LookupAtom("prolog"));
  *CurrentModulePtr = MkIntTerm(0);
  ModuleName[1] = MkAtomTerm(LookupAtom("user"));
  InitCPred("$current_module", 2, p_current_module, SafePredFlag|SyncPredFlag);
  InitCPred("$current_module", 1, p_current_module1, SafePredFlag|SyncPredFlag);
  InitCPred("$change_module", 1, p_change_module, SafePredFlag|SyncPredFlag);
}
