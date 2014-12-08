%{
#include "parse.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>

extern "C" {
    int yywrap(void);
    int yylex(void);
}

%}

%x COMMENT

blank   [ \n\t\r]*
atom    [a-zA-Z0-9_]+

%%
{atom}  {
            yylval.str = strdup(yytext); 
            if(strcmp(yytext, "not") == 0)
                return NEG;  // NEGA是一个被使用了的标志符，所以这里需要改用NEG
            return ATOM;
        }
"|"     { return DISJ; }
":-"    { return IMPL; }
"("     { return LPAREN; }
")"     { return RPAREN; }
","     { return COMMA; }
"."     { return PERIOD; }
{blank} { /* skip blank space */ }
"%"                     { BEGIN COMMENT;  }
<COMMENT>\n             { BEGIN INITIAL;  }
<COMMENT>.              {  }
%%

int yywrap() {
    return 1;
}

