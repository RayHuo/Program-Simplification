/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on January 13, 2015, 13:10 AM
 */

#ifndef GRS_H
#define	GRS_H

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

class GRS {
private:
    vector<Rule> program;
    set<int> ATOMS_P;           // Atoms(program)
public:
    GRS();
    GRS(vector<Rule> P);
    ~GRS();
    
    set<int> UCP(set<int> L);   // Algorithm 1 中的uc_P(L)函数
    
    // 通过计算 Atoms(P) \ UC_P(X \cup L)的极大不动点得到greatest reliable set。
    set<int> calGRS();
};

#endif /* GRS_H */