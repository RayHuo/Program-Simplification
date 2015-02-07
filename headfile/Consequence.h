/* 
 * File:   Consequence.h
 * Author: ray
 *
 * Created on January 13, 2015, 12:47 AM
 */

#ifndef CONSEQUENCE_H
#define	CONSEQUENCE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include "Rule.h"
#include "Loop.h"
#include "Vocabulary.h"
#include "structs.h"
#include "DependenceGraph.h"
#include "Utils.h"

using namespace std;

class Consequence {
private:
    vector<Rule> program;       // 输入的程序P
    set< set<int> > clauses;    // 程序P对应的命题公式clauses
    set<int> Atoms_P;           // Atoms(P)
    set<int> consequence;       // 最后的结果
    bool conflict;              // 判断是否出现矛盾
    
    int cal_phi_times;          // 辅助程序耗时判断，记录每步中调用phi的次数
    double upCost;              // 记录up操作的总耗时
    double gusCost;             // 记录gus操作的总耗时
public:
    Consequence();
    Consequence(vector<Rule> p);
    ~Consequence();
    

//    set< set<int> > P2Clauses();        // 把输入的程序P变成对应的命题公式(clauses)，即把rules转成DNFs。
    
    set<int> Lit();//计算Lit

    set<int> UnitPropagation(set< set<int> > clauses);// 计算clauses 的 unit propagation 结果
    set<int> UnitPropagation(set<int> literals, set< set<int> > clauses);      // 计算clauses w.r.t literals 的 unit propagation 结果
    set< set<int> > assign(set<int> literals, set< set<int> > clauses);        // 即UP(\Gamma)中的assgin方法
    
    // 下面程序中用到的程序P都是类属性program
    set<int> GUS(FILE* out, vector<Rule> P, set<int> L);                   // 计算并返回greatest unfounded set
    set<int> PhiL(FILE* out, vector<Rule> P, set<int> L, set<int> X);      // \Phi_L(X)算子
    set<int> lfp_PhiL(FILE* out, vector<Rule> P, set<int> L);              // 计算 \Phi_L(X) \cup { p | p \in L} 的极小不动点。
    
    set<int> W_P(FILE* out, set<int> L);                   // WP(L) = UP(L, P) \cup ~GUS(P, L) 算子
    set<int> lfp_WP(FILE* out);                          // 计算WP(L)的极小不动点。
    
    set<int> Lookahead(FILE* out);                 // 进行计算consequence中第4步的lookahead
    
    // 整合调用上面的函数来计算consequence
    set<int> calConsequence(FILE* out);
    
    // 辅助函数
    void printClauses(FILE* out);       // 输出clauses
};

#endif	/* CONSEQUENCE_H */
