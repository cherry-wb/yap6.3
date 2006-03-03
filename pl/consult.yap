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
* File:		consult.yap						 *
* Last rev:	8/2/88							 *
* mods:									 *
* comments:	Consulting Files in YAP					 *
*									 *
*************************************************************************/

%
% SWI options
% autoload(true,false)
% derived_from(File) -> make
% encoding(Enconding)
% expand({true,false)
% if(changed,true,not_loaded)
% imports(all,List)
% qcompile(true,false)
% silent(true,false)  => implemented
% stream(Stream)  => implemented
% consult(consult,reconsult) 
%
load_files(Files,Opts) :-
	'$load_files'(Files,Opts,load_files(Files,Opts)).

'$load_files'(Files,Opts,Call) :-		    
	'$process_lf_opts'(Opts,Silent,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,Files,Call),
	'$check_use_module'(Call,UseModule),
        '$current_module'(M0),
	'$lf'(Files,M0,Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,UseModule),
	'$close_lf'(Silent).

'$process_lf_opts'(V,_,_,_,_,_,_,_,_,_,_,_,Call) :-
	var(V), !,
	'$do_error'(instantiation_error,Call).
'$process_lf_opts'([],_,_,_,_,_,_,_,_,_,_,_,_).
'$process_lf_opts'([Opt|Opts],Silent,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,Files,Call) :-
	'$process_lf_opt'(Opt,Silent,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,Files,Call), !, 
	'$process_lf_opts'(Opts,Silent,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,Files,Call).
'$process_lf_opts'([Opt|Opts],_,_,_,_,_,_,_,_,_,_,_,Call) :-
	'$do_error'(domain_error(unimplemented_option,Opt),Call).

'$process_lf_opt'(autoload(true),_,InfLevel,_,_,_,_,_,_,_,_,_,_) :-
	get_value('$verbose_auto_load',VAL),
	(VAL = true ->
	    InfLevel = informational
	;
	    InfLevel = silent
	).
'$process_lf_opt'(autoload(false),_,_,_,_,_,_,_,_,_,_,_,_).
'$process_lf_opt'(derived_from(File),_,_,_,_,_,_,_,_,_,_,Files,Call) :-
	( atom(File) -> true ;  '$do_error'(type_error(atom,File),Call) ),
	( atom(Files) -> true ;  '$do_error'(type_error(atom,Files),Call) ),
	/* call make */
	'$do_error'(domain_error(unimplemented_option,derived_from),Call).
'$process_lf_opt'(encoding(Encoding),_,_,_,_,_,_,_,_,_,_,_,Call) :-
	'$do_error'(domain_error(unimplemented_option,encoding),Call).
'$process_lf_opt'(expand(true),_,_,true,_,_,_,_,_,_,_,_,Call) :-
	'$do_error'(domain_error(unimplemented_option,expand),Call).
'$process_lf_opt'(expand(false),_,_,false,_,_,_,_,_,_,_,_,_).
'$process_lf_opt'(if(changed),_,_,_,changed,_,_,_,_,_,_,_,_).
'$process_lf_opt'(if(true),_,_,_,true,_,_,_,_,_,_,_,_).
'$process_lf_opt'(if(not_loaded),_,_,_,not_loaded,_,_,_,_,_,_,_,_).
'$process_lf_opt'(imports(all),_,_,_,_,_,_,_,_,_,_,_).
'$process_lf_opt'(imports(Imports),_,_,_,_,_,Imports,_,_,_,_,_,_).
'$process_lf_opt'(qcompile(true),_,_,_,_,true,_,_,_,_,_,_,Call) :-
	'$do_error'(domain_error(unimplemented_option,qcompile),Call).
'$process_lf_opt'(qcompile(false),_,_,_,_,false,_,_,_,_,_,_).
'$process_lf_opt'(silent(true),Silent,silent,_,_,_,_,_,_,_,_,_,_) :-
	( get_value('$lf_verbose',Silent) -> true ;  Silent = informational),
	set_value('$lf_verbose',silent).
'$process_lf_opt'(skip_unix_comments,_,_,_,_,_,_,_,_,skip_unix_comments,_,_,_).
'$process_lf_opt'(silent(false),_,_,_,_,_,_,_,_,_,_,_,_).
'$process_lf_opt'(consult(reconsult),_,_,_,_,_,_,_,_,_,reconsult,_,_).
'$process_lf_opt'(consult(consult),_,_,_,_,_,_,_,_,_,consult,_,_).
'$process_lf_opt'(stream(Stream),_,_,_,_,_,_,Stream,_,_,_,Files,_) :-
/*	( '$stream'(Stream) -> true ;  '$do_error'(domain_error(stream,Stream),Call) ), */
	( atom(Files) -> true ;  '$do_error'(type_error(atom,Files),Call) ).

'$check_use_module'(use_module(_),use_module(_)) :- !.
'$check_use_module'(use_module(_,_),use_module(_)) :- !.
'$check_use_module'(use_module(M,_,_),use_module(M)) :- !.
'$check_use_module'(_,load_files) :- !.

'$lf'(V,_,Call,_,_,_,_,_,_,_,_,_) :- var(V), !,
	'$do_error'(instantiation_error,Call).
'$lf'([],_,_,_,_,_,_,_,_,_,_,_,_) :- !.
'$lf'(M:X, _, Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,UseModule) :- !,
	(
	  atom(M)
	->
	  '$lf'(X, M, Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,UseModule)
	  ;
	  '$do_error'(type_error(atom,M),Call)
	).
'$lf'([F|Fs], Mod,Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,UseModule) :- !,
	'$lf'(F,Mod,Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,_),
	'$lf'(Fs, Mod,Call,InfLevel,Expand,Changed,CompilationMode,Imports,Stream,Encoding,SkipUnixComments,Reconsult,UseModule).
'$lf'(X, Mod, Call,InfLevel,_,Changed,CompilationMode,Imports,Stream,_,Reconsult,SkipUnixComments,UseModule) :-
        nonvar(Stream), !,
	'$do_lf'(X, Mod, Stream, InfLevel,CompilationMode,Imports,SkipUnixComments,Reconsult,UseModule).
'$lf'(user, Mod, Call,InfLevel,_,Changed,CompilationMode,Imports,_,_,SkipUnixComments,Reconsult,UseModule) :- !,
	'$do_lf'(user_input, Mod, user_input, InfLevel, CompilationMode,Imports,SkipUnixComments,Reconsult,UseModule).
'$lf'(user_input, Mod, Call,InfLevel,_,Changed,CompilationMode,Imports,_,_,SkipUnixComments,Reconsult,UseModule) :- !,
	'$do_lf'(user_input, Mod, user_input, InfLevel, CompilationMode,Imports,Reconsult,UseModule).
'$lf'(X, Mod, Call, InfLevel,_,Changed,CompilationMode,Imports,_,_,SkipUnixComments,Reconsult,UseModule) :-
	'$find_in_path'(X, Y, Call),
	'$open'(Y, '$csult', Stream, 0), !,
	'$set_changed_lfmode'(Changed),
	'$start_lf'(X, Mod, Stream, InfLevel, CompilationMode, Imports, Changed,SkipUnixComments,Reconsult,UseModule),
	'$close'(Stream).
'$lf'(X, _, Call, _, _, _, _, _, _, _, _, _, _) :-
	'$do_error'(permission_error(input,stream,X),Call).

'$set_changed_lfmode'(true) :- !.
'$set_changed_lfmode'(_).

'$start_lf'(_, Mod, Stream,_ ,_, Imports, not_loaded, _, _, _) :-
	'$file_loaded'(Stream, Mod, Imports), !.
'$start_lf'(_, Mod, Stream, _, _, Imports, changed, _, _, _) :-
	'$file_unchanged'(Stream, Mod, Imports), !.
'$start_lf'(X, Mod, Stream, InfLevel, CompilationMode, Imports, _, SkipUnixComments, Reconsult, UseModule) :-
	'$do_lf'(X, Mod, Stream, InfLevel, CompilationMode, Imports, SkipUnixComments, Reconsult, UseModule).

'$close_lf'(Silent) :- 
	nonvar(Silent), !,
	set_value('$lf_verbose',Silent).
'$close_lf'(_).

ensure_loaded(Fs) :-
	'$load_files'(Fs, [if(changed)],ensure_loaded(Fs)).

compile(Fs) :-
	'$load_files'(Fs, [], compile(Fs)).

consult(Fs) :-
	'$has_yap_or',
	'$do_error'(context_error(consult(Fs),clause),query).
consult(V) :-
	var(V), !,
	'$do_error'(instantiation_error,consult(V)).
consult(M0:Fs) :- !,
	'$consult'(Fs, M0).
consult(Fs) :-
	'$current_module'(M0),
	'$consult'(Fs, M0).

'$consult'(Fs,Module) :-
	'$access_yap_flags'(8, 2), % SICStus Prolog compatibility
	!,
	'$load_files'(Module:Fs,[],Fs).
'$consult'(Fs, Module) :- var(V), !,
	'$load_files'(Module:Fs,[consult(consult)],Fs).

reconsult(Fs) :-
	'$load_files'(Fs, [], reconsult(Fs)).

use_module(F) :-
	'$load_files'(F, [if(not_loaded)], use_module(F)).

use_module(F,Is) :-
	'$load_files'(F, [if(not_loaded),imports(Is)], use_module(F,Is)).

use_module(M,F,Is) :-
	'$load_files'(F, [if(not_loaded),imports(Is)], use_module(M,F,Is)).

'$csult'(V, _) :- var(V), !,
	'$do_error'(instantiation_error,consult(V)).
'$csult'([], _).
'$csult'([-F|L], M) :- !, '$load_files'(M:F, [],[-M:F]), '$csult'(L, M).
'$csult'([F|L], M) :- '$consult'(F, M), '$csult'(L, M).

'$do_lf'(F, ContextModule, Stream, InfLevel, _, Imports, SkipUnixComments, Reconsult, UseModule) :-
	'$record_loaded'(Stream, M),
	'$current_module'(OldModule,ContextModule),
	getcwd(OldD),
	get_value('$consulting_file',OldF),
	'$set_consulting_file'(Stream),
	H0 is heapused, '$cputime'(T0,_),
	'$current_stream'(File,_,Stream),
	'$fetch_stream_alias'(OldStream,'$loop_stream'),
	'$change_alias_to_stream'('$loop_stream',Stream),
	get_value('$consulting',Old),
	set_value('$consulting',false),
	'$consult_infolevel'(InfLevel),
	recorda('$initialisation','$',_),
	( Reconsult = reconsult ->
	    '$start_reconsulting'(File),
	    '$start_consult'(Reconsult,File,LC),
	    '$remove_multifile_clauses'(File),
	    StartMsg = reconsulting,
	    EndMsg = reconsulted
	    ;
	    '$start_consult'(Reconsult,File,LC),
	    StartMsg = consulting,
	    EndMsg = consulted
	),
	'$print_message'(InfLevel, loading(StartMsg, File)),
	( recorded('$trace', on, TraceR) -> erase(TraceR) ; true),
	( SkipUnixComments == skip_unix_comments ->
	    '$skip_unix_comments'(Stream)
	;
	    true
	),
	'$loop'(Stream,Reconsult),
	'$end_consult',
	( nonvar(TraceR) -> recorda('$trace', on, _) ; true),
	( 
	    Reconsult = reconsult ->
	    '$clear_reconsulting'
	;
	    true
	),
	set_value('$consulting',Old),
	set_value('$consulting_file',OldF),
	cd(OldD),
	'$current_module'(Mod,OldModule),
	'$bind_module'(Mod, UseModule),
	'$import_to_current_module'(File, ContextModule, Imports),
	( LC == 0 -> prompt(_,'   |: ') ; true),
	H is heapused-H0, '$cputime'(TF,_), T is TF-T0,
	'$print_message'(InfLevel, loaded(EndMsg, File, Mod, T, H)),
	'$exec_initialisation_goals',
	'$change_alias_to_stream'('$loop_stream',OldStream),
	!.

'$bind_module'(_, load_files).
'$bind_module'(Mod, use_module(Mod)).

'$import_to_current_module'(File, M, Imports) :-
	recorded('$module','$module'(File,NM,Ps),_), M \= NM, !,
	'$use_preds'(Imports, Ps, NM, M).
'$import_to_current_module'(_, _, _).

'$consult_infolevel'(InfoLevel) :- nonvar(InfoLevel), !.
'$consult_infolevel'(InfoLevel) :-
	get_value('$lf_verbose',InfoLevel), InfoLevel \= [], !.
'$consult_infolevel'(informational).

'$start_reconsulting'(F) :-
	recorda('$reconsulted','$',_),
	recorda('$reconsulting',F,_).

'$initialization'(V) :-
	var(V), !,
	'$do_error'(instantiation_error,initialization(V)).
'$initialization'(C) :- number(C), !,
	'$do_error'(type_error(callable,C),initialization(C)).
'$initialization'(C) :- db_reference(C), !,
	'$do_error'(type_error(callable,C),initialization(C)).
'$initialization'(G) :-
	'$show_consult_level'(Level1),
	% it will be done after we leave the current consult level.
	Level is Level1-1,
	recorda('$initialisation',do(Level,G),_),
	fail.
'$initialization'(_).

'$exec_initialisation_goals' :-
	recorded('$blocking_code',_,R),
	erase(R),
	fail.
% system goals must be performed first 
'$exec_initialisation_goals' :-
	recorded('$system_initialisation',G,R),
	erase(R),
	G \= '$',
	call(G),
	fail.
'$exec_initialisation_goals' :-
	'$show_consult_level'(Level),
	recorded('$initialisation',do(Level,G),R),
	erase(R),
	G \= '$',
	'$current_module'(M),
	'$system_catch'(once(M:G), M, Error, user:'$LoopError'(Error, top)),
	'$do_not_creep',
	fail.
'$exec_initialisation_goals'.

'$include'(V, _) :- var(V), !,
	'$do_error'(instantiation_error,include(V)).
'$include'([], _) :- !.
'$include'([F|Fs], Status) :- !,
	'$include'(F, Status),
	'$include'(Fs, Status).
'$include'(X, Status) :-
	get_value('$lf_verbose',Verbosity),
	'$find_in_path'(X,Y,include(X)),
	'$values'('$included_file',OY,Y),
	'$current_module'(Mod),
	H0 is heapused, '$cputime'(T0,_),
	( '$open'(Y,'$csult',Stream,0), !,
		'$print_message'(Verbosity, loading(including, Y)),
		'$loop'(Stream,Status), '$close'(Stream)
	;
		'$do_error'(permission_error(input,stream,Y),include(X))
	),
	H is heapused-H0, '$cputime'(TF,_), T is TF-T0,
	'$print_message'(Verbosity, loaded(included, Y, Mod, T, H)),
	set_value('$included_file',OY).

'$do_startup_reconsult'(X) :-
	( '$access_yap_flags'(15, 0) ->
	  '$system_catch'(load_files(X, []),Module,Error,'$Error'(Error))
	;
	  set_value('$verbose',off),
	  load_files(X, [silent(true),skip_unix_comments])
	),
	( '$access_yap_flags'(15, 0) -> true ; halt).

'$skip_unix_comments'(Stream) :-
	'$peek'(Stream, 0'#), !, % 35 is ASCII for #
	'$get0_line_codes'(Stream, _),
	'$skip_unix_comments'(Stream).
'$skip_unix_comments'(_).


prolog_load_context(_, _) :-
	get_value('$consulting_file',[]), !, fail.
prolog_load_context(directory, DirName) :- 
	getcwd(DirName).
prolog_load_context(file, FileName) :- 
	get_value('$included_file',IncFileName),
	( IncFileName = [] ->
	  get_value('$consulting_file',FileName)
        ;
           FileName = IncFileName
        ).
prolog_load_context(module, X) :-
	'$current_module'(X).
prolog_load_context(source, FileName) :-
	get_value('$consulting_file',FileName).
prolog_load_context(stream, Stream) :- 
	'$fetch_stream_alias'(Stream,'$loop_stream').
prolog_load_context(term_position, Position) :- 
	'$fetch_stream_alias'(Stream,'$loop_stream').
	stream_position(Stream, Position).


% if the file exports a module, then we can
% be imported from any module.
'$file_loaded'(Stream, M, Imports) :-
	'$file_name'(Stream, F),
	'$ensure_file_loaded'(F, M, Imports).

'$ensure_file_loaded'(F, M, Imports) :-
	recorded('$module','$module'(F1,NM,P),_),
	recorded('$lf_loaded','$lf_loaded'(F1,_,Age),R),
	'$same_file'(F1,F), !,
	'$use_preds'(Imports,P, NM, M).
'$ensure_file_loaded'(F, M, _) :-
	recorded('$lf_loaded','$lf_loaded'(F1,M,Age),R),
	'$same_file'(F1,F).

% if the file exports a module, then we can
% be imported from any module.
'$file_unchanged'(Stream, M, Imports) :-
	'$file_name'(Stream, F),
	'$ensure_file_unchanged'(F, M, Imports).

'$ensure_file_unchanged'(F, M, Imports) :-
	recorded('$module','$module'(F1,NM,P),_),
	recorded('$lf_loaded','$lf_loaded'(F1,_,Age),R),
	'$same_file'(F1,F), !,
	'$file_is_unchanged'(F, R, Age),
	'$use_preds'(Imports, P, NM, M).
'$ensure_file_unchanged'(F, M, _) :-
	recorded('$lf_loaded','$lf_loaded'(F1,M,Age),R),
	'$same_file'(F1,F), !,
	'$file_is_unchanged'(F, R, Age).

'$file_is_unchanged'(F, R, Age) :-
        '$file_age'(F,CurrentAge),
         ((CurrentAge = Age ; Age = -1)  -> true; erase(R), fail).



path(Path) :- findall(X,'$in_path'(X),Path).

'$in_path'(X) :- recorded('$path',Path,_),
		atom_codes(Path,S),
		( S = ""  -> X = '.' ;
		  atom_codes(X,S) ).

add_to_path(New) :- add_to_path(New,last).

add_to_path(New,Pos) :-
	atom(New), !,
	'$check_path'(New,Str),
	atom_codes(Path,Str),
	'$add_to_path'(Path,Pos).

'$add_to_path'(New,_) :- recorded('$path',New,R), erase(R), fail.
'$add_to_path'(New,last) :- !, recordz('$path',New,_).
'$add_to_path'(New,first) :- recorda('$path',New,_).

remove_from_path(New) :- '$check_path'(New,Path),
			recorded('$path',Path,R), erase(R).

'$check_path'(At,SAt) :- atom(At), !, atom_codes(At,S), '$check_path'(S,SAt).
'$check_path'([],[]).
'$check_path'([Ch],[Ch]) :- '$dir_separator'(Ch), !.
'$check_path'([Ch],[Ch,A]) :- !, integer(Ch), '$dir_separator'(A).
'$check_path'([N|S],[N|SN]) :- integer(N), '$check_path'(S,SN).

% add_multifile_predicate when we start consult
'$add_multifile'(Name,Arity,Module) :-
	get_value('$consulting_file',File),
	'$add_multifile'(File,Name,Arity,Module).

'$add_multifile'(File,Name,Arity,Module) :-
	recorded('$multifile_defs','$defined'(File,Name,Arity,Module), _), !,
	'$print_message'(warning,declaration((multifile Module:Name/Arity),ignored)).
'$add_multifile'(File,Name,Arity,Module) :-
	recordz('$multifile_defs','$defined'(File,Name,Arity,Module),_), !,
	fail.
'$add_multifile'(File,Name,Arity,Module) :-
	recorded('$mf','$mf_clause'(File,Name,Arity,Module,Ref),R),
	erase(R),
	'$erase_clause'(Ref,Module),
	fail.
'$add_multifile'(_,_,_,_).

% retract old multifile clauses for current file.
'$remove_multifile_clauses'(FileName) :-
	recorded('$multifile_defs','$defined'(FileName,_,_,_),R1),
	erase(R1),
	fail.
'$remove_multifile_clauses'(FileName) :-
	recorded('$mf','$mf_clause'(FileName,_,_,Module,Ref),R),
	'$erase_clause'(Ref, Module),
	erase(R),
	fail.
'$remove_multifile_clauses'(_).

'$set_consulting_file'(user) :- !,
	set_value('$consulting_file',user_input).
'$set_consulting_file'(user_input) :- !,
	set_value('$consulting_file',user_input).
'$set_consulting_file'(Stream) :-
	'$file_name'(Stream,F),
	set_value('$consulting_file',F),
	'$set_consulting_dir'(F).

%
% Use directory where file exists
%
'$set_consulting_dir'(F) :-
	file_directory_name(F, Dir),
	cd(Dir).

'$record_loaded'(Stream, M) :-
	Stream \= user,
	Stream \= user_input,
	'$file_name'(Stream,F),
	( recorded('$lf_loaded','$lf_loaded'(F,M,_),R), erase(R), fail ; true ),

	'$file_age'(F,Age),
	recorda('$lf_loaded','$lf_loaded'(F,M,Age),_),
	fail.
'$record_loaded'(_, _).

