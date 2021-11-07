(*Abstract Syntax Tree and functions for printing

note from hao:

when you write your parse.mly, running `ocamlyacc parser.mly`
is a convenient way to verify the grammar working properly,
which also relates to the LRM. If ocamlyacc runs properly, that
means your grammar is designed without fallacies.

do we want type of function? if it doesn't return anything then we
need void?


    | IF LPAREN expr RPAREN stmt %prec NOELSE {If($3,$5,Block([]))}
    | IF LPAREN expr RPAREN stmt ELSE stmt {IF($3,$5,$7)}

*)

type op = Add | Sub | Mult | Div | Equal | Neq | And | Or 

type uop = Not | Neg

type typ = Int | Bool

type bind = typ * string

type expr =
    Literal of int
    | Fliteral of string
    | BoolLit of bool
    | Id of string
    | Binop of expr * op * expr
    | Unop of uop * expr
    | Assign of string * expr
    | Call of string * expr list
    | Noexpr

type stmt = 
    Block of stmt list
    | Expr of expr 
    | Return of expr 
    | If of expr * stmt * stmt

type func_decl = {
    typ : typ;
    funcname : string;
    formals : bind list;
    locals : bind list;
    body : stmt list;
}

type program = bind list * func_decl list

