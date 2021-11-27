/* Ocamlyacc parser for JEoMC */

%{
open Ast
%}

%token LPAREN RPAREN LBRACE RBRACE COMMA ASSIGN
%token INT
%token <string> ID
%token EOF

%start program
%type <Ast.program> program

%right ASSIGN

%%

program:
    decls EOF {$1}

decls:
    decls vdecl {(($2 :: fst $1), snd $1)}
    | decls fdecl {(fst $1, ($2 :: snd $1))}

fdecl:
    typ ID LPAREN formal_list RPAREN LBRACE vdecl_list stmt_list RBRACE
        {{typ = $1;
          fname = $2;
          formals = List.rev $4;
          locals = List.rev $7;
          body = List.rev $8}}

formal_list:
    typ ID {[($1,$2)]}
    | formal_list COMMA typ ID {($3,$4) :: $1}

typ:
    INT {Int}

vdecl_list:
    vdecl_list vdecl { $2 :: $1 }

vdecl:
    typ ID {($1,$2)}

stmt_list:
    stmt_list stmt {$2 :: $1}

stmt:
    expr {Expr $1}
    | LBRACE stmt_list RBRACE {Block(List.rev $2)}

expr:
    ID {Id($1)}
    | ID ASSIGN expr {Assign($1,$3)}
    | LPAREN expr RPAREN {$2}


