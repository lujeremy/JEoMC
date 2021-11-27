(* Semantic checking *)

open Ast
open Sast

module StringMap = Map.Make(String)

let check (globals, functions) =
    let check_binds (kind : string) (binds : bind list) =
        let rec dups = function
            [] -> ()
            | ((_,n1) :: (_,n2) :: _) when n1 = n2 ->
                raise (Failure ("Duplicate " ^ kind ^ " " ^ n1))
            | _ :: t -> dups t 
        in dups (List.sort (fun (_,a) (_,b) -> compare a b) binds)
    in

    check_binds "global" globals;

    let built_in_decls =
        let add_bind map (name, ty) = StringMap.add name {
            typ = Int; (* double check this type *)
            fname = name;
            formals = [(ty,"x")];
            locals = []; body = []
        } map
        in List.fold_left add_bind StringMap.empty [ ("print", Int);
                                                ] (*add real funcs*)
    in 

    let add_func map fd =
        let built_in_err = "Function " ^ fd.fname ^ " may not be defined"
            and dup_err = "Duplicate function " ^ fd.fname
            and make_err err = raise (Failure err)
            and n = fd.fname
        in match fd with
            _ when StringMap.mem n built_in_decls -> make_err built_in_err
            | _ when StringMap.mem n map -> make_err dup_err
            | _ -> StringMap.add n fd map 
    in

    let function_decls = List.fold_left add_func built_in_decls functions
    in

    let find_func s =
        try StringMap.find s function_decls
        with Not_found -> raise (Failure ("Unrecognized function " ^ s))
    in

    let _ = find_func "main" in

    let check_function func =
        check_binds "formal" func.formals;
        check_binds "local" func.locals;

        let check_assign lvaluet rvaluet err =
            if lvaluet = rvaluet then lvaluet else raise (Failure err)
        in 

        let symbols = List.fold_left (fun m (ty, name) -> StringMap.add name ty m)
                      StringMap.empty (globals @ func.locals)
        in

        let type_of_identifier s =
            try StringMap.find s symbols
            with Not_found -> raise (Failure ("Undeclared identifier " ^ s))
        in 

        let rec expr = function 
            Id s -> (type_of_identifier s, SId s)
            | Assign(var, e) as ex ->
                let lt = type_of_identifier var
                and (rt, e') = expr e in
                let err = "Illegal assignment " ^ string_of_typ lt ^ " = " ^
                    string_of_typ rt ^ " in " ^ string_of_expr ex
                in (check_assign lt rt err, SAssign(var, (rt, e')))
        in

        let rec check_stmt = function 
            Expr e -> SExpr (expr e)
            | Block sl ->
                let rec check_stmt_list = function 
                    Block sl :: ss -> check_stmt_list (sl @ ss)
                    | s :: ss -> check_stmt s :: check_stmt_list ss 
                    | [] -> []
                in SBlock(check_stmt_list sl)
        in {
            styp = func.typ;
            sfname = func.fname;
            sformals = func.formals;
            slocals = func.locals;
            sbody = match check_stmt (Block func.body) with
            SBlock(sl) -> sl
            | _ -> raise (Failure ("Internal error: Block error"))
        }
    in (globals, List.map check_function functions)










