#include "NLP.h"
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


/*
 * 默认构造函数
 */
NLP::NLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
}


/*
 * 自定义构造函数
 */
NLP::NLP(vector<Rule> p, set<int> l) : program(p), L(l) {
    Atoms_P.clear();
    for(vector<Rule>::const_iterator pit = program.begin(); pit != program.end(); pit++) {
        Atoms_P.insert(pit->heads.begin(), pit->heads.end());   // 因为head中必然是非负的
        for(set<int>::const_iterator bit = pit->bodys.begin(); bit != pit->bodys.end(); bit++) {
            Atoms_P.insert(abs(*bit));
        }
    }
}


/*
 * 析构函数
 */
NLP::~NLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
}


/*
 * 论文中的T^w_P,L(X)函数。
 */
set<int> NLP::TWPL(set<int> X, FILE* out) {
    
}
   

/*
 * 论文中的T^s_P,L(X)函数。
 */
set<int> NLP::TSPL(set<int> X, FILE* out) {
    
}
   

/*
 * 论文中的lfp(T^w_P,L(X)) = GWRS(P,L)，这里本应该是有参数P和L的，但实际是直接使用program和L这两个属性
 */
set<int> NLP::TW(FILE* out) {
    
}
 

/*
 * 论文中的lfp(T^s_P,L(X)) = GSRS(P,L)，这里本应该是有参数P和L的，但实际是直接使用program和L这两个属性
 */
set<int> NLP::TS(FILE* out) {
    
}
  

/*
 * 直接调用TW()
 */
set<int> NLP::GWRS(FILE* out) {
    
}
 

/*
 * 直接调用TS()
 */
set<int> NLP::GSRS(FILE* out) {
    
}