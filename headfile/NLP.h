/* 
 * File:   NLP.h
 * Author: ray
 *
 * Created on January 30, 2015, 12:47 AM
 */

#ifndef NLP_H
#define	NLP_H

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

class NLP {
private:
    vector<Rule> program;
    set<int> Atoms_P;
    set<int> L;
    set<int> NL;        // { ~p | ~p \in L},  即 L \ L+，其中L+ = {p | p \in L}，注意论文中的 L- = {p | ~p \in L}，与这里的NL不一样
public:
    NLP();
    NLP(vector<Rule> p, set<int> l);
    ~NLP();
    
    set<int> TWPL(set<int> X, FILE* out);          // 论文中的T^w_P,L(X)函数。
    set<int> TSPL(set<int> X, FILE* out);          // 论文中的T^s_P,L(X)函数。
    set<int> TW(FILE* out);                        // 论文中的lfp(T^w_P,L(X)) = GWRS(P,L)，这里本应该是有参数P和L的，但实际是直接使用program和L这两个属性
    set<int> TS(FILE* out);                        // 类似TW()
    set<int> GWRS(FILE* out);                      // 直接调用TW()
    set<int> GSRS(FILE* out);                      // 直接调用TS()
};

#endif /* NLP_H */