/* Coloque aqui definições regulares */

/* Whitespace: espaço, tab ou new line */
WS  [ \t\n]

/* Dígito: qualquer número de 0 a 9 */
D  [0-9]

/* Letra: Qualquer caractere de A a Z, case insensitive */
L  [A-Za-z]

/* _ID: opcionalmente uma letra, um "_" ou um "$";
        em seguida qualquer combinação de letras, dígitos e "_"
*/
_ID  ([$A-Za-z_]?[A-Za-z0-9_]+)

/* _INT: uma sequência de pelo menos um dígito */
_INT  {D}+

/* _FLOAT: pelo menos um dígito; em seguida opcionalmente
           um ponto e pelo menos um dígito (ponto flutuante);
           em seguida opcionalmente um "E" ou um "e",
           seguido opcionalmente de um "+" ou um "-", seguido
           de pelo menos um dígito
*/
_FLOAT  {D}+(\.{D}+)?((E|e)(\+|-)?{D}+)?

/* _FOR: a palavra "for", case insensitive */
_FOR ([Ff][Oo][Rr])

/* _IF: a palavra "if", case insensitive */
_IF ([Ii][Ff])

/* _MAIG: um ">" seguido de um "=" */
_MAIG >=

/* _MEIG: um "<=" seguido de um "=" */
_MEIG <=

/* _IG: um "=" seguido de um "=" */
_IG ==

/* _DIF: um "!" seguido de um "=" */
_DIF !=

/* _COMENTARIO: um "/" e um "*", seguido de 
*/
_COMENTARIO ((\/\*([^\*]|\*[^\/])*\*\/)|(\/\/[^\n]*))

_STRING (("([^\n"\\]|""|\\")*")|('([^\n'\\]|''|\\')*'))

_STRING2 (`[^`]*`)

%%
    /* Padrões e ações. Nesta seção, comentários devem ter um tab antes */

{WS}	        { /* ignora espaços, tabs e '\n' */ }

{_FOR}          { return _FOR; }

{_INT}          { return _INT; }

{_FLOAT}        { return _FLOAT; }

{_IF}           { return _IF; }

{_MAIG}         { return _MAIG; }

{_MEIG}         { return _MEIG; }

{_IG}           { return _IG; }

{_DIF}          { return _DIF; }

{_STRING}       { return _STRING; }

{_STRING2}       { return _STRING2; }

{_COMENTARIO}   { return _COMENTARIO; }

{_ID}           { return _ID; }

.       { return *yytext; 
          /* Essa deve ser a última regra. Dessa forma qualquer caractere isolado será retornado pelo seu código ascii. */ }

%%

/* Não coloque nada aqui - a função main é automaticamente incluída na hora de avaliar e dar a nota. */