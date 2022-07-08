DELIM  [\n\t\r ]

DIGITO [0-9]

LETRA  [A-Za-z_]

DOUBLE {DIGITO}+("."{DIGITO}+)?

ID     {LETRA}({LETRA}|{DIGITO})*

STRING (\"((\"\")|(\\\")|([^\"\n]))*\")|(\'((\'\')|(\\\')|([^\'\n]))*\')

%%

{DELIM}    { }
     
{DOUBLE}   { yylval.c = tk(yytext); 
             return NUM; }

"let"	   { return LET; }

"var"	   { return VAR; }

"if"	   { return IF; }

"else"	   { return ELSE; }

"while"	   { return WHILE; }

"for"	   { return FOR; }

"=="       { return IG; }

">="       { return MAIG; }

"<="       { return MEIG; }

"!="       { return DIF; }

"+="       { return SOMAATR; }

"++"       { return INCR; }

"--"       { return DECR; }

"{}"       { return NEWOBJ; }

"[]"       { return NEWARR; }

{ID}       { yylval.c = tk(yytext); 
             return ID; }

{STRING}   { yylval.c = tk(yytext); 
             return STRING; }

.          { yylval.c = tk((string) yytext);
             return yytext[0]; }

%%