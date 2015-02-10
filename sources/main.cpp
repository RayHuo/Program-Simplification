/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on December 8, 2014, 12:47 AM
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "structs.h"
#include "Rule.h"
#include "Vocabulary.h"
#include "Utils.h"
#include "DependenceGraph.h"
#include "Loop.h"
#include "MaxU.h"
#include "Consequence.h"
#include "GRSNLP.h"
#include "GRSDLP.h"
#include "NLP.h"
#include "DLP.h"

using namespace std;

//#define MAXU
//#define GRST
//#define GRS_WS
//#define WFM
#define SIMPLIFICATION

extern vector<Rule> G_Rules;    // 保存输入文件的所有rules，定义在global.cpp中
extern FILE* yyin;              // lex.cpp中定义的变量，默认的文件输入对象，注意此处不要重定义
extern void yyparse();          // parse.cpp中定义的函数，实际进行parser的函数，把输入文件中的rules写进G_Rules并把相关的atom存放在Vocabulary中
FILE* fout;                     // 自定义的输出文件对象。
FILE* foutP1;                   // 输出程序中的P1
FILE* foutLandGWRS;             // 保留L和GWRS


/*
 * This is the Program for both NLP and DLP simplification
 */
int main(int argc, char** argv) {
//    string inputfile = "IO/inputs/2009/channelRouting/channelRoute.in10.in";
//    yyin = fopen("IO/inputs/DLP/RandomQuantifiedBooleanFormulas/Ql2k3alpha5.00rho0.8-79-2.cnf.dl.lparse", "r");
    yyin =fopen(argv[1], "r");
    if(!yyin) {
        printf("IO Error : Cannot open the input file!\n");
        exit(0);
    }
    

    string filename(argv[1]);
    filename.replace(3, 6, "outputs");
    
//    fout = fopen((filename + ".out").c_str(), "w");
    fout = fopen((filename + "_P2.out").c_str(), "w");
    if(!fout) {
        printf("IO Error : Cannot open the output file!\n");
        exit(0);
    }
    
    // 存放P1的文件
    foutP1 = fopen((filename + "_P1.out").c_str(), "w");
    if(!foutP1) {
        printf("IO Error : Cannot open the foutP1 file!\n");
        exit(0);
    }
    
    foutLandGWRS = fopen((filename + "_LandGWRS").c_str(), "w");
    if(!foutLandGWRS) {
        printf("IO Error : Cannot open the foutLandGWRS file!\n");
        exit(0);
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
    
    // 计算NLP的greatest reliable set，注意输入文件是否NLP
//    GRSNLP grs(G_Rules);
//    set<int> ans = grs.calGRS(fout);
    
    // 计算DLP的greatest reliable set，注意输入文件是否DLP
    Consequence consequence(G_Rules);
    set<int> L = consequence.calConsequence(fout);
    fprintf(fout, "\nFinal Consequence : ");
    for(set<int>::iterator cit = L.begin(); cit != L.end(); cit++) {
        int id = *cit;
            if(id < 0) {
                fprintf(fout, "~");
                id *= -1;
            }
            fprintf(fout, "%s", Vocabulary::instance().getAtomName(id));
            if(cit != --(L.end()))
                fprintf(fout, ", ");
    }
    fprintf(fout, "\n");
    
    GRSDLP grsdlp(G_Rules, L);
    set<int> result = grsdlp.calGRSDLP(fout);
    fprintf(fout, "\nGRSDLP : ");
    for(set<int>::const_iterator rit = result.begin(); rit != result.end(); rit++) {
        if(*rit < 0)
            fprintf(fout, "~");
        fprintf(fout, "%s ", Vocabulary::instance().getAtomName(abs(*rit)));
    }
    fprintf(fout, "\n");
    
    
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
    

#ifdef GRS_WS
    
    /**
     * 当前比较耗时的地方应该是lookahead
     */
    
    fprintf(fout, "Calculating the Greatest Reliable Set :\n"); fflush(fout);
//    Vocabulary::instance().VocabularyDetails(fout);
    fprintf(fout, "Vocabulary atom list size = %d\n", Vocabulary::instance().atomsNumber() - 1);
    
    // 计算NLP的最大consequence，注意输入文件是否NLP
    Consequence consequence(G_Rules);
    set<int> L = consequence.calConsequence(fout);
    fprintf(fout, "\nFinal Consequence : ");
    for(set<int>::iterator cit = L.begin(); cit != L.end(); cit++) {
        int id = *cit;
            if(id < 0) {
                fprintf(fout, "~");
                id *= -1;
            }
            fprintf(fout, "%s", Vocabulary::instance().getAtomName(id));
            if(cit != --(L.end()))
                fprintf(fout, ", ");
    }
    fprintf(fout, "\nFinal Consequence size = %d\n", L.size());fflush(fout);
    
    
    int type = atoi(argv[2]);       // 0 for NLP, 1 for DLP
    
    
    // 在计算GWRS和GSRS的过程中，需要忽略原程序中的所有fact，
    // 因为这些fact相当于是WFM中已经算出来的部分，必须去除这部分后，GWRS和GSRS不为空才有意义。
    // 而去掉这些fact的操作是在进行GWRS的计算过程中就忽略掉这些fact，所以这里弄一个没有fact的
    vector<Rule> inputRules;
    for(vector<Rule>::const_iterator grit = G_Rules.begin(); grit != G_Rules.end(); grit++) {
        if(grit->type != FACT)
            inputRules.push_back(*grit);
    }
    
    
    // NLP 当前情况：顺利跑完，中间逻辑貌似没错，但尚未详细检查输出的过程！！！！！！！！
    // 计算NLP的greatest strong(and weak) reliable set，注意输入文件是否NLP
    if(type == 0) {
        NLP nlp(inputRules, L);
        long gwrsStart = clock();
        set<int> gwrs = nlp.GWRS(fout);
        long gwrsEnd = clock();
        double gwrsCost = (double)(gwrsEnd - gwrsStart) / CLOCKS_PER_SEC; // 得到的耗时单位为秒
        fprintf(fout, "\nThe GWRS of NLP is : ");
        for(set<int>::const_iterator it = gwrs.begin(); it != gwrs.end(); it++) {
            if(*it < 0)
                fprintf(fout, "~");
            fprintf(fout, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(fout, "\nThe GWRS size of NLP = %d\n", gwrs.size());        fflush(fout);
        fprintf(fout, "The GWRS cost time = %.3f\n", gwrsCost);             fflush(fout);

        long gsrsStart = clock();
        set<int> gsrs = nlp.GSRS(fout);
        long gsrsEnd = clock();
        double gsrsCost = (double)(gsrsEnd - gsrsStart) / CLOCKS_PER_SEC;
        fprintf(fout, "\nThe GSRS of NLP is : ");
        for(set<int>::const_iterator it = gsrs.begin(); it != gsrs.end(); it++) {
            if(*it < 0)
                fprintf(fout, "~");
            fprintf(fout, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(fout, "\nThe GSRS size of NLP = %d\n", gsrs.size());        fflush(fout);
        fprintf(fout, "The GSRS cost time = %.3f\n", gsrsCost);             fflush(fout);
    }
    
    
    
    // DLP 当前情况：顺利跑完，中间逻辑貌似没错，但尚未详细检查输出的过程！！！！！！！！
    // 计算DLP的greatest strong(and weak) reliable set，注意输入文件是否DLP
    if(type == 1) {
        DLP dlp(inputRules, L);
        set<int> gwrs = dlp.GWRS(fout);
        fprintf(fout, "\nThe GWRS of DLP is : ");
        for(set<int>::const_iterator it = gwrs.begin(); it != gwrs.end(); it++) {
            if(*it < 0)
                fprintf(fout, "~");
            fprintf(fout, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(fout, "\nThe GWRS of DLP size = %d\n", gwrs.size());        fflush(fout);

        set<int> gsrs = dlp.GSRS(fout);
        fprintf(fout, "\nThe GSRS of DLP is : ");
        for(set<int>::const_iterator it = gsrs.begin(); it != gsrs.end(); it++) {
            if(*it < 0)
                fprintf(fout, "~");
            fprintf(fout, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(fout, "\nThe GSRS of DLP size = %d\n", gsrs.size());        fflush(fout);
    }
    
#endif    
    
    
#ifdef WFM
    int facts = 0;
    for(vector<Rule>::const_iterator it = G_Rules.begin(); it != G_Rules.end(); it++) {
        if(it->type == FACT)
            facts++;
    }
    fprintf(fout, "WFM number which also the facts number = %d\n", facts);
#endif    
    

#ifdef SIMPLIFICATION
    /**
     * 1）将那些consequence作为约束，加入到P，得到P1;  
     * 2）通过GWRS化简P，将剩下的consequence作为约束加入到化简后的程序中，得到P2;
     * 3）然后使用clasp/claspD求解程序P1和P2，比较两者的速度，我们的目标是P2更快些。
     */
    
    // 计算出consequence L
    Consequence consequence(G_Rules);
    set<int> L = consequence.calConsequence(fout);
    
    fprintf(foutLandGWRS, "L = \n");
    for(set<int>::const_iterator it = L.begin(); it != L.end(); it++) {
        if(*it < 0)
            fprintf(foutLandGWRS, "not ");
        fprintf(foutLandGWRS, "%s", Vocabulary::instance().getAtomName(abs(*it)));
        if(it != --(L.end())) {
            fprintf(foutLandGWRS, ", ");
        }
    }
    fprintf(foutLandGWRS, "\n\nL size = %d\n\n", L.size());
    
    
    // P1
    for(vector<Rule>::const_iterator pit = G_Rules.begin(); pit != G_Rules.end(); pit++) {
        pit->output(foutP1);
    }
    fprintf(foutP1, "\n");
    for(set<int>::const_iterator lit = L.begin(); lit != L.end(); lit++) {
        fprintf(foutP1, ":- ");
        if(*lit < 0) 
            fprintf(foutP1, "not ");
        fprintf(foutP1, "%s.\n", Vocabulary::instance().getAtomName(abs(*lit)));
    }
    
    
    // P2
    int type = atoi(argv[2]);       // 0 for NLP, 1 for DLP
    
    // 去掉fact，计算GWRS
    vector<Rule> inputRules;
    for(vector<Rule>::const_iterator grit = G_Rules.begin(); grit != G_Rules.end(); grit++) {
        if(grit->type != FACT)
            inputRules.push_back(*grit);
    }
    
    set<int> gwrs;      gwrs.clear();
    
    // NLP 当前情况：顺利跑完，中间逻辑貌似没错，但尚未详细检查输出的过程！！！！！！！！
    // 计算NLP的greatest strong(and weak) reliable set，注意输入文件是否NLP
    if(type == 0) {
        NLP nlp(inputRules, L);
        gwrs = nlp.GWRS(fout);
    }
    // DLP 当前情况：顺利跑完，中间逻辑貌似没错，但尚未详细检查输出的过程！！！！！！！！
    // 计算DLP的greatest strong(and weak) reliable set，注意输入文件是否DLP
    if(type == 1) {
        DLP dlp(inputRules, L);
        gwrs = dlp.GWRS(fout);
    }
    
    
    fprintf(foutLandGWRS, "GWRS = \n");
    for(set<int>::const_iterator it = gwrs.begin(); it != gwrs.end(); it++) {
        if(*it < 0)
            fprintf(foutLandGWRS, "not ");
        fprintf(foutLandGWRS, "%s", Vocabulary::instance().getAtomName(abs(*it)));
        if(it != --(L.end())) {
            fprintf(foutLandGWRS, ", ");
        }
    }
    fprintf(foutLandGWRS, "\n\nGWRS size = %d\n", gwrs.size());
    
    
    vector<Rule> origin = G_Rules;
//    vector<Rule> origin = inputRules;
    // 获取P2
    Utils::tr_p(origin, gwrs);          // 第一个参数是取引用的
    
    set<int> L_gwrs;    // 理论上，计算出来的GWRS比L要小，这里计算 L - GWRS 作为约束加入到化简过的程序中
    set_difference(L.begin(), L.end(), gwrs.begin(), gwrs.end(), inserter(L_gwrs, L_gwrs.begin()));
    
    // P2中第一部分：基于GWRS化简过后的程序P'
    for(vector<Rule>::const_iterator it = origin.begin(); it != origin.end(); it++) {
        it->output(fout);
    }
    fprintf(fout, "\n");
    
    // P2中第二部分：L - GWRS 作为约束加入到程序中
    for(set<int>::const_iterator it = L_gwrs.begin(); it != L_gwrs.end(); it++) {
        fprintf(fout, ":- ");
        if(*it < 0)
            fprintf(fout, "not ");
        fprintf(fout, "%s.\n", Vocabulary::instance().getAtomName(abs(*it)));
    }
    
    
#endif    
    
    fclose(foutP1);
    fclose(foutLandGWRS);
    fclose(fout);
    fclose(yyin);
    
    return 0;
}

