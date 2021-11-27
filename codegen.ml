(*

    Code generator: takes a semantically checked AST and produces LLVM IR

*)

module L = Llvm
module A = Ast
open Sast

module StringMap = Map.Make(String)

let translate (globals, functions) =
    let context = L.global_context () in 

    let the_module = L.create_module context "JEoMC" in 

    let i32_t = L.i32_type context
    and i8_t = L.i8_type context in

    let ltype_of_typ = function 
        A.Int -> i32_t 
    in 

    let global_vars : L.llvalue StringMap.t =
        let global_var m (t,n) =
            let init = match t with
                _ -> L.const_int (ltype_of_typ t) 0
            in StringMap.add n (L.define_global n init the_module) m in 
        List.fold_left global_var StringMap.empty globals in 

    let printf_t : L.lltype =
        L.var_arg_function_type i32_t [| L.pointer_type i8_t |] in 
    let printf_func : L.llvalue =
        L.declare_function "printf" printf_t the_module in 

    let function_decls : (L.llvalue * sfunc_decl) StringMap.t =
        let function_decl m fdecl =
            let name = fdecl.sfname
            and formal_types =
            Array.of_list (List.map (fun (t,_) -> ltype_of_typ t) fdecl.sformals)
            in let ftype = L.function_type (ltype_of_typ fdecl.styp) formal_types in
            StringMap.add name (L.define_function name ftype the_module, fdecl) m in 
        List.fold_left function_decl StringMap.empty functions in 

    let build_function_body fdecl =
        let (the_function, _) = StringMap.find fdecl.sfname function_decls in 
        let builder = L.builder_at_end context (L.entry_block the_function) in 

        let int_format_str = L.build_global_stringptr "%d\n" "fmt" builder in 

        let local_vars =
            let add_formal m (t,n) p =
                L.set_value_name n p;

    let local = L.build_alloca (ltype_of_typ t) n builder in 
        ignore (L.build_store p local builder);
        StringMap.add n local m 

        and add_local m (t,n) =
    let local_var = L.build_alloca (ltype_of_typ t) n builder
    in StringMap.add n local_var m 
        in 

        let formals = List.fold_left2 add_formal StringMap.empty fdecl.sformals
            (Array.to_list (L.params the_function)) in 
        List.fold_left add_local formals fdecl.slocals
    in

    let lookup n = try StringMap.find n local_vars with Not_found -> StringMap.find n global_vars
    in

    let rec expr builder ((_,e) : sexpr) = match e with
        SId s -> L.build_load (lookup s) s builder
        | SAssign (s,e) -> let e' = expr builder e in 
            ignore(L.build_store e' (lookup s) builder); e'
    in

    let add_terminal builder instr =
        match L.block_terminator (L.insertion_block builder) with
            Some _ -> ()
            | None -> ignore (instr builder) in 

    let rec stmt builder = function 
        SBlock sl -> List.fold_left stmt builder sl 
        | SExpr e -> ignore(expr builder e); builder

    in 

    let builder = stmt builder (SBlock fdecl.sbody) in 
    add_terminal builder (match fdecl.styp with
        t -> L.build_ret (L.const_int (ltype_of_typ t) 0))
    in

    List.iter build_function_body functions;
    the_module


















