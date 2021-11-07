{ open Parser }

let lowercase_letter = ['a'-'z']

rule tokenize =
    parse [' ' '\t' '\r' '\n'] { tokenize lexbuf }
        | '+' { PLUS }
        | '-' { MINUS }
        | '*' { TIMES }
        | '/' { DIVIDE }
        | '=' { ASSIGN }
        | "if" { IF }
        | "then" { THEN }
        | "else" { ELSE }
        | ';' { SEQ }
        | lowercase_letter+ as var { VAR(var) }
        | ['0'-'9']+ as lit { LITERAL(int_of_string lit) }
        | eof { EOF }
