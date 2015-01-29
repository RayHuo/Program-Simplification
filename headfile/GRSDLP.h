/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on January 26, 2015, 17:23 AM
 */

#ifndef GRSDLP_H
#define	GRSDLP_H

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

class GRSDLP {
private:
    vector<Rule> program;
    set<int> Atoms_P;           // Atoms(P)
    set<int> L;                 // 从前面步骤计算得到的consequence
    int num;
public:
    GRSDLP();
    GRSDLP(vector<Rule> p, set<int> l);
    ~GRSDLP();
    
    set<int> UCLP(set<int> U, set<int> X, FILE* out);   // 实现论文中的algorithm2
    set<int> RSLP(set<int> U, FILE* out);               // 计算RSLP(U)的函数
    set<int> gfp_RSLP(FILE* out);                       // 计算gfp(RSLP(U))的函数，实际计算结果就是DLP的greatest reliable set
    set<int> calGRSDLP(FILE* out);                      // 通过计算极大不动点来求得DLP的greatest reliable set的过程
};

#endif /* GRSDLP_H */