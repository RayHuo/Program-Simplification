#include "Consequence.h"
#include "Rule.h"
#include "Loop.h"
#include "Vocabulary.h"
#include "structs.h"
#include "DependenceGraph.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;


/*
 * 默认构造函数
 */
Consequence::Consequence() {
    
}


/*
 * 自定义构造函数
 */
Consequence::Consequence(vector<Rule> p) {
    
}


/*
 * 析构函数
 */
Consequence::~Consequence() {
    
}



/*
 * 把输入的程序P变成对应的命题公式(clauses)，即把rules转成DNFs。
 */
set< set<int> > Consequence::P2Clauses() {        

}


/*
 * 计算clauses w.r.t literals 的 unit propagation 结果
 */
set<int> Consequence::UnitPropagation(set<int> literals, set< set<int> > clauses) {
    set<int> conq;
    
    return conq;
}  


/*
 * 即UP(\Gamma)中的assgin方法
 */
set< set<int> > Consequence::assign(set<int> literals, set< set<int> > clauses) {

}


/*
 * 计算并返回greatest unfounded set
 */
set<int> Consequence::GUS(vector<Rule> P, set<int> L) {
    
}


/*
 * \Phi_L(X)算子
 */
set<int> Consequence::PhiL(set<int> L, set<int> X) {
    
}


/*
 * 计算 \Phi_L(X) \cup { p | p \in L} 的极小不动点。
 */
set<int> Consequence::lfp_PhiL() {

}


/*
 * WP(L) = UP(L, P) \cup ~GUS(P, L) 算子
 */
set<int> Consequence::W_P(set<int> L) {
    
}


/*
 * 计算WP(L)的极小不动点。
 */
set<int> Consequence::lfp_WP() {

}


/*
 * 进行计算consequence中第4步的lookahead
 */
bool Consequence::Lookahead(set<int> L) {
    
}


/*
 * 整合调用上面的函数来计算consequence
 */
set<int> Consequence::calConsequence() {
    
}