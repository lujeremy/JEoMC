{ open Parser }

let lowercase_letter = ['a'-'z']

rule tokenize =
    parse [' ' '\t' '\r' '\n'] { tokenize lexbuf }
        | '=' { ASSIGN }
        | ['a'-'z' 'A'-'Z']['a'-'z' 'A'-'Z' '0'-'9' '_']* as lxm { ID(lxm) }
        | eof { EOF }
