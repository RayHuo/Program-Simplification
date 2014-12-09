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
    for(vector<Rule>::iterator it = G_Rules.begin(); it != G_Rules.end(); it++) {
//         it->Situation(fout);         // 直接把rule中atom的id输出来。 
        it->output(fout);       // 把rule以实际形式输出来。
    }
     
    // 找出输入程序的正依赖图中的所有SCC。
    DependenceGraph dpdg(G_Rules);       // 直接使用默认构造函数就会构造输入程序的正依赖图了。
    dpdg.findSCCs();            // 找出正依赖图中的所有SCC，目前的算法没有找到所有孤立点形成的SCC。
    dpdg.printDpdGraph(fout);   // 打印出输入程序的正依赖图
    dpdg.printSCCs(fout);       // 输出找到的SCC。
    
    
#endif    
    
    
    fclose(fout);
    fclose(yyin);
    
    return 0;
}

