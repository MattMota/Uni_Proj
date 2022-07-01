DELIM   [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
DOUBLE  {DIGITO}+("."{DIGITO}+)?
ID      {LETRA}({LETRA}|{DIGITO})*
COMENTARIO ((\/\*([^\*]|\*[^\/])*\*\/)|(\/\/[^\n]*))
STRING (\"((\"\")|(\\\")|([^\"\n]))*\")|(\'((\'\')|(\\\')|([^\'\n]))*\')

%%

{DELIM}    { }

             
{DOUBLE}   { yylval.c = yytext; 
             return NUM; }

"let"	   { yylval.c = "&"; 
             return LET; }

"var"	   { yylval.c = "var"; 
             return VAR; }

"if"	   { yylval.c = "if"; 
             return IF; }

"else"	   { yylval.c = "if"; 
             return ELSE; }

{ID}       { yylval.c = yytext; 
             return ID; }

"=="       { yylval.c = "=="; 
             return IG; }

">="       { yylval.c = ">="; 
             return MAIG; }

"<="       { yylval.c = "<="; 
             return MEIG; }

"!="       { yylval.c = "!="; 
             return DIF; }

.          { yylval.c = yytext;
	     return yytext[0]; }

%%