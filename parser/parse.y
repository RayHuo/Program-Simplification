%{
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include <vector>
#include "structs.h"
#include "Utils.h"
#include "Vocabulary.h"
#include "Rule.h"

extern "C" {
    void yyerror(const char *s);
    extern int yylex(void);
}

extern vector<Rule> G_Rules;

%}

%union {
    char* str;  // 这个作用于lex.l中的yylval.str。
    int i;
    struct __rule* r;
    struct __literals* l;
}

%token<str> DISJ;
%token<str> ATOM;
%token<str> NEG;
%token<str> IMPL;
%token<str> LPAREN;
%token<str> RPAREN;
%token<str> COMMA;
%token<str> PERIOD;

%type<str> term terms atom
%type<i> literal
%type<r> rule
%type<l> head body

%left IMPL;
%left DISJ;

%%

program : rules {
            printf("Start Parser!\n");
        }
        |
;

rules : rules rule {
            Rule r($2);
            G_Rules.push_back(r);
      }
      | rule {
            Rule r($1);
            G_Rules.push_back(r);
      }
;

rule : head PERIOD {
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head_length = 0;
        $$->body_length = 0;
        $$->type = FACT;
        for(int i = 0; i < $1->atoms_length; i++)
            $$->head[$$->head_length++] = $1->atoms[i];
        // think about the initailization of $$->body.
     }
     | head IMPL PERIOD {
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head_length = 0;
        $$->body_length = 0;
        $$->type = FACT;
        for(int i = 0; i < $1->atoms_length; i++)
            $$->head[$$->head_length++] = $1->atoms[i];
     }
     | head IMPL body PERIOD {
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head_length = 0;
        $$->body_length = 0;
        $$->type = RULE;
        
        for(int i = 0; i < $1->atoms_length; i++)
            $$->head[$$->head_length++] = $1->atoms[i];
        for(int i = 0; i < $3->atoms_length; i++)
            $$->body[$$->body_length++] = $3->atoms[i];
     }
     | IMPL body PERIOD {
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head_length = 0;
        $$->body_length = 0;
        $$->type = CONSTRANT;
        for(int i = 0; i < $2->atoms_length; i++)
            $$->body[$$->body_length++] = $2->atoms[i];
     }
;

head : head DISJ atom {
        $$ = (_literals*)malloc(sizeof(_literals));
        $$->atoms_length = 0;

        $1->atoms[$1->atoms_length++] = Vocabulary::instance().findAtom($3);
        for(int i = 0; i < $1->atoms_length; i++)
          $$->atoms[$$->atoms_length++] = $1->atoms[i];
     }
     | atom {
        $$ = (_literals*)malloc(sizeof(_literals));
        $$->atoms_length = 0;
        $$->atoms[$$->atoms_length++] = Vocabulary::instance().findAtom($1);
     }
;

body : body COMMA literal {
        $$ = (_literals*)malloc(sizeof(_literals));
        $$->atoms_length = 0;

        $1->atoms[$1->atoms_length++] = $3;
        for(int i = 0; i < $1->atoms_length; i++)
          $$->atoms[$$->atoms_length++] = $1->atoms[i];
     }
     | literal {
        $$ = (_literals*)malloc(sizeof(_literals));
        $$->atoms_length = 0;
        $$->atoms[$$->atoms_length++] = $1;
     }
;

literal : NEG atom {
            int id = Vocabulary::instance().findAtom($2);
            if(id < 0)
                id = Vocabulary::instance().insertAtom($2);
            $$ = -1 * id;
        }
        | atom {
            int id = Vocabulary::instance().findAtom($1);
            if(id < 0)
                id = Vocabulary::instance().insertAtom($1);
            $$ = id;
        }
;

atom : ATOM LPAREN terms RPAREN {
        char str_buff[1024];
        sprintf(str_buff, "%s(%s)", $1, $3);
        $$ = strdup(str_buff);
        // 获取到一个atom后，马上将其插入到Vocabulary中。
        int id = Vocabulary::instance().insertAtom($$);
     }
     | ATOM {
        $$ = strdup($1);
        // 获取到一个atom后，马上将其插入到Vocabulary中。
        int id = Vocabulary::instance().insertAtom($$);
     }
;

terms : terms COMMA term {
        char str_buff[1024];
        sprintf(str_buff, "%s, %s", $1, $3);
        $$ = strdup(str_buff);
      }
      | term {
        $$ = strdup($1);
      }
;

term : ATOM {
        $$ = strdup($1);
     }
;

%%

void yyerror(const char *s) {
    printf("Parser Error : %s\n", s);
}