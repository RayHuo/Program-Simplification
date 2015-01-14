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
    program.clear();
    clauses.clear();
    Atoms_P.clear();
    consequence.clear();
    conflict = false;
}


/*
 * 自定义构造函数
 */
Consequence::Consequence(vector<Rule> p) : program(p) {
    clauses.clear();    // 通过p获取对应的clauses    
    Atoms_P.clear();    // 通过p获取对应的Atoms(P)
    for(vector<Rule>::iterator pit = program.begin(); pit != program.end(); pit++) {
        set<int> clause = Utils::rule2DNF(*pit);
        clauses.insert(clause);
        
        set<int> lits;
        set_union((pit->heads).begin(), (pit->heads).end(), (pit->bodys).begin(), 
                (pit->bodys).end(), inserter(lits, lits.begin()));
        Atoms_P.insert(lits.begin(), lits.end());
    }
    
    consequence.clear();
    conflict = false;
}


/*
 * 析构函数
 */
Consequence::~Consequence() {
    program.clear();
    clauses.clear();
    Atoms_P.clear();
    consequence.clear();
    conflict = false;
}



/*
 * 把输入的程序P变成对应的命题公式(clauses)，即把rules转成DNFs。
 * 貌似没有用了
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
 * \Phi_L(X) = {a | 存在r \in P, a \in head(r) 且 a \in {p | ~p \in L},
 *                  body(r) \cap ({p | ~p \in L} \cup {~p | p \in L}) = \emptyset,
 *                  body^+(r) \subseteq (X \ {p | ~p \in L}), head(r) \cap L = \emptyset}
 */
set<int> Consequence::PhiL(set<int> L, set<int> X) {
    set<int> phi;
    for(vector<Rule>::iterator pit = program.begin(); pit != program.end(); pit++) {
        set<int> head_L;
        set_intersection((pit->heads).begin(), (pit->heads).end(), L.begin(), L.end(), inserter(head_L, head_L.begin()));
        if(head_L.empty()) {    // head(r) \cap L = \emptyset
            set<int> Ls;    // 对应 {p | ~p \in L} \cup {~p | p \in L}
            for(set<int>::iterator lit = L.begin(); lit != L.end(); lit++)
                Ls.insert((*lit)*(-1));
            set<int> body_L;
            set_intersection((pit->bodys).begin(), (pit->bodys).end(), Ls.begin(), Ls.end(), inserter(body_L, body_L.begin()));
            if(body_L.empty()) {    // body(r) \cap ({p | ~p \in L} \cup {~p | p \in L}) = \emptyset
                set<int> NL;
                for(set<int>::iterator lit = L.begin(); lit != L.end(); lit++)
                    if(*lit < 0)
                        NL.insert((*lit)*(-1));
                set<int> X_NL;
                set_difference(X.begin(), X.end(), NL.begin(), NL.end(), inserter(X_NL, X_NL.begin()));
                set<int> pbody_L;
//                includes();
            }
        }
    }
    
    return phi;
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