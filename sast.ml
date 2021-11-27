open Ast

type sexpr = typ * sx
and sx = 
    SId of string
    | SAssign of string * sexpr

type sstmt =
    SBlock of sstmt list
    | SExpr of sexpr

type sfunc_decl = {
    styp : typ;
    sfname : string;
    sformals : bind list;
    slocals : bind list;
    sbody : sstmt list;
}

type sprogram = bind list * sfunc_decl list
