%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include js.l

using namespace std;

Atributos {
  vector<string> c;
}

#define YYSTYPE Atributos

int yylex();
int yyparse();
void yyerror(const char *);

%}

%token DELIM DIGITO LETRA DOUBLE ID NUM LET VAR IF ELSE IG MAIG MEIG DIF

%right '=' 
%nonassoc '<' '>' IG
%left '+' '-'
%left '*' '/' '%'
%right '^'

// Start indica o símbolo inicial da gramática
%start S

%%

S : CMDs { cout << $1.c << "." << endl; }
  ;

CMDs : CMD ';' CMDs   { $$.c = $1.c + "\n" + $3.c; }
     |                { $$.c = ""; }
     ;

CMD : CMDIF
    | CMDIFELSE
    | CMDWHILE
    | CMDFOR
    | DECLET
    | DECVAR
    ;

CMDIF : IF EXP '{' CMDs '}' { $$.c = $2.c + " ? " + $2.c + " "  + $4.c + "=" + "^"; }
      ;
           
CMDIFELSE : IF EXP '{' CMDs '}' ELSE CMDs
            { $$.c = $2.c + " " + gera_label("if_true") + " ? " + $7.c + " "  + resolve_endereco + "=" + "^"; }
          ;

CMDWHILE : WHILE EXP '{' CMDs '}'
           { $$.c = ; }
         ;

CMDFOR : FOR '(' EXP ',' EXP ',' EXP ')' '{' CMDs '}'
           { $$.c = ; }
         ;

// Declarações

DEC : DECLET
    | DECVAR
    ;

DECLET : LET ID '=' OP1 { $$.c = $2.c + " & " + $2.c + " "  + $4.c + "=" + "^"; }
        | LET ID         { $$.c = $2.c + " &"; }
        ;

DECVAR : VAR ID '=' OP1 { $$.c = $2.c + " & " + $2.c + " "  + $4.c + "=" + "^"; }
        | VAR ID         { $$.c = $2.c + " &"; }
        ;

// Derivações done

// Expressões (equações)

// IG, MAIG, MEIG funcionam?
EXP : LVALUE IG OP1
    | OP1 MAIG OP1
    | OP1 MEIG OP1
    | OP1 DIF OP1
    | OP1
    ;

// Operadores

OP1 : OP1 '+' OP2 { $$.c = $1.c + " " + $3.c + " +"; }
    | OP1 '-' OP2 { $$.c = $1.c + " " + $3.c + " -"; }
    | OP1 '*' OP2 { $$.c = $1.c + " " + $3.c + " *"; }
    | OP1 '/' OP2 { $$.c = $1.c + " " + $3.c + " /"; }
    | OP2
    ;

OP2 : FIN '^' OP2 { $$.c = $1.c + " " + $3.c + " ^"; }
    | FIN
    ;

// Terminais (exceto para IDs pq podem ter campos)

FIN : LVALUE          { $$.c = $1.c + " @"; }
    | NUM             { $$.c = $1.c; }
    | STR             { $$ = $1; }
    | ID '(' ')'      { $$.c = $1.c + " #"; }
    | ID '(' ARGs ')' { $$.c = $3.c + " " + $1.c + " #"; }
    | '(' OP1 ')'     { $$ = $2; }
    |

ARGs : EXP ',' ARGs { $$.c = $1.c + " " + $3.c; }
     | EXP
     ;

// Tratamento de left values

LVALUE : ID LVALUEPROP { $$.c = $1.c + "@ " + $2.c; }

LVALUEPROP :
    | '[' EXP ']' { $$.c = $2.c; }
    | LVALUEPROP '.' ID { $$.c = $3.c + "[@]"; }
    |
    ;

%%

#include "lex.yy.c"

void yyerror( const char* st ) {
   puts( st ); 
   printf( "Proximo a: %s\n", yytext );
   exit( 1 );
}

// Operadores para string

vector<string> operator+( vector<string> a, vector<string> b ) {
  return concatena( a, b );
}

vector<string> operator+( vector<string> a, string b ) {
  a.push_back( b );
  return a;
}

// Funções de label

string gera_label( string prefixo ) {
  static int n = 0;
  return prefixo + "_" + to_string( ++n ) + ":";
}

vector<string> resolve_enderecos( vector<string> entrada ) {
  map<string,int> label;
  vector<string> saida;
  for( int i = 0; i < entrada.size(); i++ ) 
    if( entrada[i][0] == ':' ) 
        label[entrada[i].substr(1)] = saida.size();
    else
      saida.push_back( entrada[i] );
  
  for( int i = 0; i < saida.size(); i++ ) 
    if( label.count( saida[i] ) > 0 )
        saida[i] = to_string(label[saida[i]]);
    
  return saida;
}

// Main

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}