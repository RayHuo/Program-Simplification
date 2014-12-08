/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on December 8, 2014, 12:47 AM
 */

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <vector>
#include <assert.h>
#include "structs.h"
#include "Rule.h"
#include "Vocabulary.h"
#include "Utils.h"
#include "DependenceGraph.h"

using namespace std;

#define INPUTDETAIL

extern vector<Rule> G_Rules;
extern FILE* yyin;
extern void yyparse();
FILE* fout;

/*
 * This is the Program for both NLP and DLP simplification
 */
int main(int argc, char** argv) {
    yyin = fopen("IO/inputs/sample1.in", "r");
    if(!yyin) {
        printf("IO Error : Cannot open the input file!\n");
        assert(0);
    }
    
    fout = fopen("IO/outputs/sample1.out", "w");
    if(!fout) {
        printf("IO Error : Cannot open the output file!\n");
        assert(0);
    }
    
    yyparse();
    printf("End Parser!\n");
    
#ifdef INPUTDETAIL    
    Vocabulary::instance().VocabularyDetails(fout);
    
    fprintf(fout, "\nThe Input file with %d rules, is :\n", G_Rules.size());
    for(vector<Rule>::iterator it = G_Rules.begin(); it != G_Rules.end(); it++)
//         it->Situation(fout);
        it->output(fout);
     
#endif    
    
    
    fclose(fout);
    fclose(yyin);
    
    return 0;
}

