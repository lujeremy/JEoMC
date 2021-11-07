open Ast

(*module Hashtbl = Hashtbl.Make(String);;*)
let vals = Hashtbl.create 10;;

let rec eval = function 
    Lit(x)            -> x
  | Binop(e1, op, e2) ->
      let v1  = eval e1 in
      let v2 = eval e2 in
          (match op with
            Add -> v1 + v2
              | Sub -> v1 - v2
              | Mul -> v1 * v2
              | Div -> v1 / v2)
  | Asg(var, expr) ->
        let r = eval expr in Hashtbl.add vals var r; r
  | Seq(e1, e2) -> ignore (eval e1); eval e2
  | Var(v) -> Hashtbl.find vals v
  | If(e1, e2, e3) ->
        if (eval e1 != 0) then eval e2 else eval e3

let _ =
  let lexbuf = Lexing.from_channel stdin in
  let expr = Parser.expr Scanner.tokenize lexbuf in
  let result = eval expr in
  print_endline (string_of_int result)
