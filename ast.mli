(* Abstract Syntax Tree *)

type typ = Int

type bind = typ * string

type expr =
    Id of string
    | Assign of string * expr

type stmt =
    Block of stmt list
    | Expr of expr


type func_decl = {
    typ : typ;
    fname : string;
    locals : bind list;
    body : stmt list;
}

type program = bind list * func_decl list

