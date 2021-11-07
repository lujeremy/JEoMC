type operator = Add | Sub | Mul | Div

type expr =
    Binop of expr * operator * expr
  | Lit of int
  | Asg of string * expr
  | Seq of expr * expr
  | Var of string
  | If of expr * expr * expr
