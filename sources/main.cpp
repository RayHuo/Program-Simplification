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
#include "Loop.h"
#include "MaxU.h"
#include "Consequence.h"
#include "GRS.h"

using namespace std;

//#define MAXU
#define GRST

extern vector<Rule> G_Rules;    // 保存输入文件的所有rules，定义在global.cpp中
extern FILE* yyin;              // lex.cpp中定义的变量，默认的文件输入对象，注意此处不要重定义
extern void yyparse();          // parse.cpp中定义的函数，实际进行parser的函数，把输入文件中的rules写进G_Rules并把相关的atom存放在Vocabulary中
FILE* fout;                     // 自定义的输出文件对象。


/*
 * This is the Program for both NLP and DLP simplification
 */
int main(int argc, char** argv) {
    yyin = fopen("IO/inputs/samples/sample_dlp1.in", "r");
    if(!yyin) {
        printf("IO Error : Cannot open the input file!\n");
        assert(0);
    }
    
    fout = fopen("IO/outputs/samples/sample_dlp1.out", "w");
    if(!fout) {
        printf("IO Error : Cannot open the output file!\n");
        assert(0);
    }
    
    yyparse();
    printf("End Parser!\n");
    
#ifdef MAXU    
    Vocabulary::instance().VocabularyDetails(fout);
    
    fprintf(fout, "\nThe Input file with %d rules, is :\n", G_Rules.size());
    for(vector<Rule>::iterator it = G_Rules.begin(); it != G_Rules.end(); it++) {
//         it->Situation(fout);         // 直接把rule中atom的id输出来。 
        it->output(fout);       // 把rule以实际形式输出来。
    }
     
    // 这里定义一个dpdg纯粹是为了查看一下DependenceGraph的功能
    // 找出输入程序的正依赖图中的所有SCC。
    DependenceGraph dpdg(G_Rules);       // 直接使用默认构造函数就会构造输入程序的正依赖图了。
    dpdg.findSCCs();            // 找出正依赖图中的所有SCC，目前的算法没有找到所有孤立点形成的SCC。
    dpdg.printDpdGraph(fout);   // 打印出输入程序的正依赖图
    dpdg.printSCCs(fout);       // 输出找到的SCC。
    
    // 构造MaxU对象，使用算法计算最大的U。
    MaxU maxu(G_Rules);
    set<int> U = maxu.MaxUTest(fout);   // 计算得到U，输出整个计算过程。
    fprintf(fout, "\nThe max suitable U is : ");
    for(set<int>::iterator uit = U.begin(); uit != U.end(); uit++) {
        fprintf(fout, "%s", Vocabulary::instance().getAtomName(*uit));
        if(uit != --(U.end()))
            fprintf(fout, ", ");
    }
    fprintf(fout, "\n");
    
#endif    
    
    
#ifdef GRST
    fprintf(fout, "Calculating the Greatest Reliable Set :\n");
    Vocabulary::instance().VocabularyDetails(stdout);
    
    GRS grs(G_Rules);
    set<int> ans = grs.calGRS(fout);
    
    
//    Consequence consequence(G_Rules);
    
//    // Testing Unit Propagation
//    fprintf(fout, "The Unit Propagation :\n");
//    Vocabulary::instance().VocabularyDetails(stdout);
//    Consequence consequence(G_Rules);
//    set<int> result = consequence.calConsequence();
//    for (set<int>::const_iterator it = result.begin();
//            it != result.end(); ++ it) {
//        fprintf(fout, "%d ", *it);
//    }
//    fprintf(fout, "\n");

//    // Testing Greatest Unfounded Set 
//    fprintf(fout, "The Greatest Unfounded Set : \n");
//    consequence.printClauses(fout);
//    set<int> L;
//    L.insert(1);        L.insert(2);    // L = { a, c }
//    set<int> gus = consequence.GUS(fout, G_Rules, L);
//    fprintf(fout, "\nGUS(P, L) : ");
//    for(set<int>::iterator git = gus.begin(); git != gus.end(); git++) {
//        fprintf(fout, "%s", Vocabulary::instance().getAtomName(*git));
//        if(git != --(gus.end()))
//            fprintf(fout, ",");
//    }
//    fprintf(fout, "\n");        fflush(fout);

//    // Testing W_P(L)     
//    set<int> conq = consequence.lfp_WP(fout);
//    fprintf(fout, "\nConsequence : ");
//    for(set<int>::iterator cit = conq.begin(); cit != conq.end(); cit++) {
//        int id = *cit;
//            if(id < 0) {
//                fprintf(fout, "~");
//                id *= -1;
//            }
//            fprintf(fout, "%s", Vocabulary::instance().getAtomName(id));
//            if(cit != --(conq.end()))
//                fprintf(fout, ", ");
//    }
//    fprintf(fout, "\n");        fflush(fout);
    
    
//    set<int> finalConsequence = consequence.calConsequence(fout);
//    fprintf(fout, "\nFinal Consequence : ");
//    for(set<int>::iterator cit = finalConsequence.begin(); cit != finalConsequence.end(); cit++) {
//        int id = *cit;
//            if(id < 0) {
//                fprintf(fout, "~");
//                id *= -1;
//            }
//            fprintf(fout, "%s", Vocabulary::instance().getAtomName(id));
//            if(cit != --(finalConsequence.end()))
//                fprintf(fout, ", ");
//    }
//    fprintf(fout, "\n");        fflush(fout);
    
#endif
    
    fclose(fout);
    fclose(yyin);
    
    return 0;
}

