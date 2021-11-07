%{ open Ast %}

%token PLUS MINUS TIMES DIVIDE EOF ASSIGN SEQ IF THEN ELSE
%token <string> VAR
%token <int> LITERAL

%left SEQ
%right ASSIGN
%left IF THEN ELSE
%left PLUS MINUS
%left TIMES DIVIDE

%start expr
%type <Ast.expr> expr

%%

expr:
  expr PLUS   expr { Binop($1, Add, $3) }
| expr MINUS  expr { Binop($1, Sub, $3) }
| expr TIMES  expr { Binop($1, Mul, $3) }
| expr DIVIDE expr { Binop($1, Div, $3) }
| expr SEQ expr { Seq($1, $3) }
| IF expr THEN expr ELSE expr { If($2, $4, $6) }
| LITERAL          { Lit($1) }
| VAR { Var($1) }
| VAR ASSIGN expr { Asg($1, $3) }

