/* Ocamlyacc parser for JEoMC */

%{
open Ast
%}

%token LPAREN RPAREN LBRACE RBRACE ASSIGN
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
    decls vdecl {(($2 :: f $1), s $1)}
    | decls fdecl {(f $1, ($2 :: s $1))}

fdecl:
    typ ID LPAREN RPAREN LBRACE vdecl_list stmt_list RBRACE
        {{typ = $1;
          fname = $2;
          locals = List.rev $6;
          body = List.rev $7}}

vdecl:
    typ ID {($1,$2)}

typ:
    INT {Int}

vdecl_list:
    vdecl_list vdecl { $2 :: $1 }

stmt:
    expr {Expr $1}
    | LBRACE stmt_list RBRACE {Block(List.rev $2)}

stmt_list:
    stmt_list stmt {$2 :: $1}

expr:
    ID {Id($1)}
    | ID ASSIGN expr {Assign($1,$3)}
    | LPAREN expr RPAREN {$2}




