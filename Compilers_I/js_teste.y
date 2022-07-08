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

#define YYSTYPE Atributos

void add_var ( vector<string> v );
void if_declared ( vector<string> v );
void if_not_declared ( vector<string> v );
void print_vector( vector<string> v );
string gera_label( string prefixo );
vector<string> usa_label( string prefixo );
vector<string> define_label( string prefixo );
vector<string> resolve_enderecos( vector<string> entrada );
vector<string> tk( string s );
string trim_string( string s );

vector<string> operator+( vector<string> a, vector<string> b );
vector<string> operator+( vector<string> a, string b );
vector<string> operator+( string a, vector<string> );

int yylex();
int yyparse();
void yyerror(const char *);

map<string, int> var_map;
extern int yylineno;

%}

%locations

%token NUM LET VAR IF ELSE WHILE FOR IG MAIG MEIG MAI MEN DIF SOMAATR INCR DECR STRING ID

%right '=' 
%nonassoc IG MAIG MEIG MAI MEN DIF
%left '+' '-'
%left '*' '/' '%'
%right '^'

// Start indica o símbolo inicial da gramática
%start S

%%

// Start (começo da gramática)

S : CMDs { print_vector(resolve_enderecos($1.c)); }
  ;

// Comandos genéricos (linha única, multilinha e encadeados)

CMDs : // Cadeias de comandos
       CMDLINHA ';' CMDs { $$.c = $1.c + $3.c; }
     | CMDMULT CMDs { $$.c = $1.c + $2.c; }
     |              { $$.c.clear(); }
     ;

CMDLINHA: // Comandos de linha única
          ATR    { $$.c = $1.c + "^"; }
        | CMDLET { $$.c = $1.c; }
        ;

CMDMULT : // Comandos multilinha
          CMDIF    { $$.c = $1.c; }
        | CMDWHILE { $$.c = $1.c; }
        ;


// Declarações

CMDLET : LET ID '=' RVALUE MULTDECL
         { if_declared( $2.c );
           add_var( $2.c );
           $$.c = $2.c + "&" + $2.c + $4.c + "=" + "^" + $5.c;
         }
       | LET ID MULTDECL
         { if_declared( $2.c );
           add_var( $2.c );
           $$.c = $2.c + "&" + $3.c;
         }
       ;

MULTDECL : ',' ID '=' EQ MULTDECL
           { if_declared( $2.c );
             add_var( $2.c );
             $$.c = $2.c + "&" + $2.c + $4.c + "=" + "^" + $5.c;
           }
         | ',' ID MULTDECL
           { if_declared( $2.c );
             add_var( $2.c );
             $$.c = $2.c + "&" + $3.c;
           }
         | { $$.c.clear(); }
         ;

// Atribuições

ATR : // Atribuições de variáveis
      LVALUE '=' RVALUE
      { if_not_declared( $1.c );
        $$.c = $1.c + $3.c + "=";
      }
    | LVALUE SOMAATR RVALUE
      { if_not_declared( $1.c );
        $$.c = $1.c + $1.c + "@" + $3.c + "+" + "=";
      }
    | LVALUEPROP '=' RVALUE
      { $$.c = $1.c + "@" + $3.c + "[=]" + "^"; }
    ;

// Condicionais

CMDIF : IF '(' EQ ')' CMDBLOCK
        { string end_if = gera_label("end_if");
          // Is EQ false? if so, jump to "end_if"
          $$.c = $3.c + "!" + usa_label(end_if) + "?"
          // If commands; defines "end_if"
          + $5.c + define_label(end_if);
        }
      | IF '(' EQ ')' CMDBLOCK ELSE CMDELSE
        { string start_else = gera_label("start_else"), end_else = gera_label("end_else");
          // Is EQ false? If so, go to "start_else"
          $$.c = $3.c + "!" + usa_label(start_else) + "?"
          // If commands; go to "end_else"
          + $5.c + usa_label(end_else) + "#"
          // Define "start_else"; else commands; define "end_else"
          + define_label(start_else) + $7.c + define_label(end_else);
        }
      ;

CMDELSE : // Blocos de comando do "else"
          CMDBLOCK { $$.c = $1.c; }
        | CMDMULT  { $$.c = $1.c; }
        ;

CMDWHILE : WHILE '(' EQ ')' CMDBLOCK
           { string start_while = gera_label("start_while"),
              end_while = gera_label("end_while");
             // Define start while; is EQ false? If so, go to end_while
             $$.c = define_label(start_while) + $3.c + "!" + usa_label(end_while) + "?"
             // "While" commands; go to "start_while"; define "end_while"
             + $5.c + usa_label(start_while) + "#" + define_label(end_while);
           }

CMDBLOCK : // Blocos de comando do "if"
           CMDLINHA ';'     { $$.c = $1.c; }
         | '{' CMDs '}'     { $$.c = $2.c; }
         | '{' CMDs '}' ';' { $$.c = $2.c; }
         | ';'
         ;

// Equações

EQ : EQ IG EQ   { $$.c = $1.c + $3.c + "=="; }
   | EQ MAIG EQ { $$.c = $1.c + $3.c + ">="; }
   | EQ MEIG EQ { $$.c = $1.c + $3.c + "<="; }
   | EQ MAI EQ  { $$.c = $1.c + $3.c + ">"; }
   | EQ MEN EQ  { $$.c = $1.c + $3.c + "<"; }
   | EQ DIF EQ  { $$.c = $1.c + $3.c + "!="; }
   | EQ '+' EQ  { $$.c = $1.c + $3.c + "+"; }
   | EQ '-' EQ  { $$.c = $1.c + $3.c + "-"; }
   | EQ '*' FIN { $$.c = $1.c + $3.c + "*"; }
   | EQ '/' FIN { $$.c = $1.c + $3.c + "/"; }
   | FIN
   ;

// Finais/terminais

FIN : LVALUE      { $$.c = $1.c + "@"; }
    | LVALUEPROP  { $$.c = $1.c + "@"; }
    | NUM         { $$.c = ($1.c); }
    | '-' NUM     { $$.c = "0" + $2.c + "-"; }
    | '+' NUM     { $$.c = $2.c; }
    | PREOP       { $$.c = $1.c; }
    | POSTOP      { $$.c = $1.c; }
    | '(' EQ ')'  { $$ = $2; }
    | '{' '}'     { $$.c = {"{}"}; }
    | '[' ']'     { $$.c = {"[]"}; }
    | STRING      { $$.c = $1.c; }
    ;

// Left values e right values

LVALUE : ID
       ;
       
LVALUEPROP : EQ '.' ID    { $$.c = $1.c + $3.c; }
           | EQ '[' RVALUE ']'
           ;

RVALUE : EQ
       | ATR
       ;

// Operadores terminais

PREOP : // Operadores prefixos
        INCR ID { $$.c = $2.c + $2.c + "@" + "1" + "+" + "=" + "^" + $2.c + "@"; }
      | DECR ID { $$.c = $2.c + $2.c + "@" + "1" + "-" + "=" + "^" + $2.c + "@"; }
      ;

POSTOP : // Operadores pósfixos
         ID INCR { $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^"; }
       | ID DECR { $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "-" + "=" + "^"; }
       ;

%%

#include "lex.yy.c"

vector<string> tk( string s ) {
  return {s};
}

// Operadores para string

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

// Função de adicionar variáveis

void add_var ( vector<string> v ) {
  var_map[v[0]] = yylineno;
}

void if_declared ( vector<string> v ) {
  if(var_map.count(v[0]) != 0) {
    cout << ("Erro: a variável '" + v[0] + "' já foi declarada na linha " + to_string(var_map[v[0]]) + ".") << endl;
    exit( 1 );
  }
}

void if_not_declared ( vector<string> v ) {
  if(var_map.count(v[0]) == 0) {
    cerr << ("Erro: a variável '" + v[0] + "' não foi declarada." + to_string(yylineno)) << endl;
    exit( 1 );
  }
}

// Função de string

string trim_string( string s ) {
  return s.substr( 1, s.length()-2 );
}

// Funções de label

void print_vector( vector<string> v ) {
  for ( string s : v ) {
    cout << s << " ";
  }
  cout << "." << endl;
}

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

// Funções do YACC

void yyerror( const char* st ) {
   puts( st ); 
   printf( "Proximo a: %s, na linha: %d\n", yytext, yylineno );
   exit( 1 );
}

// Main

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}