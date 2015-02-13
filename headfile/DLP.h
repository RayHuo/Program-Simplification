/* 
 * File:   DLP.h
 * Author: ray
 *
 * Created on January 30, 2015, 13:13 AM
 */

#ifndef DLP_H
#define	DLP_H

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
#include <algorithm>

using namespace std;

class DLP {
private:
    vector<Rule> program;
    set<int> Atoms_P;
    set<int> L;
    set<int> NL;        // {～p | ~p \in L}，用于表示 L \ L+。这个与论文中定义的 L- = {p | ~p \in L} 不同 
    set<int> LM;        // {p | ~p \in L}，LM 即 L minus， 论文中定义的 L- = {p | ~p \in L} 
    int rswpl_time;
    int rsspl_time;
    int all_c_in_rswpl_time;
    int all_c_in_rsspl_time;
    bool rsspl_flag;
public:
    DLP();
    DLP(vector<Rule> p, set<int> l);
    ~DLP();
    
    set<int> RSWPL(set<int> X, FILE* out);      // 论文中algorithm 1的函数的weak版本。
    set<int> RSSPL(set<int> X, FILE* out, bool flag);      // 论文中algorithm 1的函数的strong版本。
    set<int> RSW(FILE* out);                    // 计算RSWPL的极小不动点
    set<int> RSS(FILE* out);                    // 计算RSSPL的极小不动点
    set<int> GWRS(FILE* out);                   // 直接调用RWS得到结果
    set<int> GSRS(FILE* out);                   // 直接调用RSS得到结果
    
    // 辅助函数
    bool all_C_in_RSWPL(set<int> X, set<int> C, FILE* out); // 用于RSWPL中判断是否 for each p \in C, C \subseteq RSWPL(X \cup {p})
    bool all_C_in_RSSPL(set<int> X, set<int> C, FILE* out); // 用于RSWPL中判断是否 for each p \in C, C \subseteq RSSPL(X \cup {p})
};

#endif /* DLP_H */