%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Atributos {
  vector<string> c;
};

vector<string> operator+( vector<string> a, vector<string> b ) {
  a.insert( a.end(), b.begin(), b.end() );
  return a;
}

vector<string> operator+( vector<string> a, string b ) {
  a.push_back( b );
  return a;
}

vector<string> operator+( string a, vector<string> b ) {
  b.insert(b.begin(), a);
  return b;
}

#define YYSTYPE Atributos

int yylex();
int yyparse();
string gera_label( string prefixo );
vector<string> usa_label( string prefixo );
vector<string> define_label( string prefixo );
vector<string> resolve_enderecos( vector<string> entrada );
void imprime_codigo( vector<string> codigo );
void yyerror(const char *);
vector<string> tk( string s );

%}

%token NUM LET VAR IF ELSE WHILE FOR IG MAIG MEIG DIF SOMAATR INCR DECR NEWOBJ NEWARR STRING ID

%right SOMAATR
%right '='
%nonassoc IG MAIG MEIG DIF '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right '^'
%left '['

%left '.'

%start RAIZ

%%

// Raiz da árvore de derivação

RAIZ : S  { imprime_codigo( resolve_enderecos($1.c + ".") ); }
     ;

S : CMD S  { $$.c = $1.c + $2.c; }
  |        { $$.c.clear(); }
  ;
  
CMD : CMD_LET ';' { $$ = $1; }
    | CMD_IF
    | CMD_WHILE
    | RVALUE ';'  { $$.c = $1.c + "^"; }
    | ';'         { $$.c.clear(); }
    ;
  
CMD_LET : LET IDs { $$ = $2; }
        ;
      
IDs : UM_ID ',' IDs  { $$.c = $1.c + $3.c; }
    | UM_ID
    ;
    
UM_ID : ID             { $$.c = $1.c + "&"; }
      | ID '=' RVALUE  
        { $$.c = $1.c + "&" + $1.c + $3.c + "=" + "^"; }
      ;

CMD_IF : IF '(' E ')' CMD_BLOCK
         { string end_if = gera_label("end_if");
           // Is EQ false? if so, jump to "end_if"
           $$.c = $3.c + "!" + usa_label(end_if) + "?"
           // If commands; defines "end_if"
           + $5.c + define_label(end_if);
         }
       | IF '(' E ')' CMD_BLOCK ELSE CMD_BLOCK
         { string start_else = gera_label("start_else"), end_else = gera_label("end_else");
           // Is EQ false? If so, go to "start_else"
           $$.c = $3.c + "!" + usa_label(start_else) + "?"
           // If commands; go to "end_else"
           + $5.c + usa_label(end_else) + "#"
           // Define "start_else"; else commands; define "end_else"
           + define_label(start_else) + $7.c + define_label(end_else);
         }
       ;

CMD_WHILE : WHILE '(' E ')' CMD_BLOCK
            { string start_while = gera_label("start_while"),
              end_while = gera_label("end_while");
              // Define start while; is EQ false? If so, go to end_while
              $$.c = define_label(start_while) + $3.c + "!" + usa_label(end_while) + "?"
              // "While" commands; go to "start_while"; define "end_while"
              + $5.c + usa_label(start_while) + "#" + define_label(end_while);
            }
          ;

CMD_BLOCK : // Blocos de comando do "if"
           CMD       { $$.c = $1.c; }
         | '{' S '}' { $$.c = $2.c; }
         ;

ATRIB : LVALUE '=' RVALUE { $$.c = $1.c + $3.c + "="; }
      | LVALUE SOMAATR RVALUE { $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; }
      | LVALUEPROP '=' RVALUE { $$.c = $1.c + $3.c + "[=]"; }
      | LVALUEPROP SOMAATR RVALUE { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; }
      ;
       
E : E '+' E   { $$.c = $1.c + $3.c + "+"; }
  | E '-' E   { $$.c = $1.c + $3.c + "-"; }
  | E '*' E   { $$.c = $1.c + $3.c + "*"; }
  | E '/' E   { $$.c = $1.c + $3.c + "/"; }
  | E '%' E   { $$.c = $1.c + $3.c + "%"; }
  | E IG E    { $$.c = $1.c + $3.c + "=="; }
  | E MAIG E  { $$.c = $1.c + $3.c + ">="; }
  | E MEIG E  { $$.c = $1.c + $3.c + "<="; }
  | E DIF E   { $$.c = $1.c + $3.c + "!="; }
  | E '<' E   { $$.c = $1.c + $3.c + "<"; }
  | E '>' E   { $$.c = $1.c + $3.c + ">"; }
  | '-' E     { $$.c = "0" + $2.c + "-"; }
  | F
  ;
  
F : NUM
  | STRING
  | PREOP
  | POSTOP
  | LVALUE     { $$.c = $1.c + "@"; }
  | LVALUEPROP { $$.c = $1.c + "[@]"; }
  | '(' RVALUE ')'  { $$.c = $2.c; }
  | NEWOBJ    { $$.c = { "{}" }; }
  | NEWARR    { $$.c = { "[]" }; }
  ;

PREOP : // Operadores prefixos
        INCR ID { $$.c = $2.c + $2.c + "@" + "1" + "+" + "=" + "^" + $2.c + "@"; }
      | DECR ID { $$.c = $2.c + $2.c + "@" + "1" + "-" + "=" + "^" + $2.c + "@"; }
      ;

POSTOP : // Operadores pósfixos
         ID INCR { $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^"; }
       | ID DECR { $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "-" + "=" + "^"; }
       ;

LVALUE : ID
       ;
       
LVALUEPROP : E '.' ID    { $$.c = $1.c + $3.c; }
           | E '[' RVALUE ']' { $$.c = $1.c + $3.c; }
           ;
    
RVALUE : E
       | ATRIB
       ;
     
%%
#include "lex.yy.c"

string gera_label( string prefixo ) {
  static int n = 0;
  return (prefixo + "_" + to_string( ++n ) + ":");
}

vector<string> usa_label( string label ) {
  return {label};
}

vector<string> define_label( string label ) {
  label = ":" + label;
  return {label};
}

vector<string> resolve_enderecos( vector<string> entrada ) {
  map<string,int> label;
  vector<string> saida;
  for( int i = 0; i < entrada.size(); i++ ) {
    if( entrada[i][0] == ':' ) 
        label[entrada[i].substr(1)] = saida.size();
    else
      saida.push_back( entrada[i] );
  }
  
  for( int i = 0; i < saida.size(); i++ ) {
    if( label.count( saida[i] ) > 0 )
        saida[i] = to_string(label[saida[i]]);
  }
    
  return saida;
}

vector<string> tk( string s ) {
  return {s};
}

void yyerror( const char* st ) {
   puts( st ); 
   printf( "Proximo a: %s\n", yytext );
   exit( 1 );
}

void imprime_codigo( vector<string> codigo ) {
  for( string instrucao : codigo )
    cout << instrucao << " ";
  cout << endl;
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}
