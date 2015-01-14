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
 * Lit(P) = Atom(P) \cup {~a | a \in Atom(P)}
 */
set<int> Consequence::Lit(set< set<int> > clauses) {
    set<int> ret;
    set<int> tmp;
    for (set< set<int> >::const_iterator it = clauses.begin();
            it != clauses.end(); ++ it) {
        tmp.insert(it->begin(), it->end());
    }
    for (set<int>::const_iterator it = tmp.begin();
            it != tmp.end(); ++ it) {
        ret.insert(*it);
        ret.insert(- *it);
    }
    return ret;
}
/*
 * UnitPropagation without literals
 */
set<int> Consequence::UnitPropagation(set<set<int> > clauses) {
    set<int> empty_set;
    if (clauses.find(empty_set) != clauses.end()) {
        // return Lit
        return Lit(clauses);
    }
    // unit clause
    set<int> unit_clause;
    for (set< set<int> >::const_iterator it = clauses.begin();
            it != clauses.end(); ++ it) {
        if (it->size() == 1) {
            unit_clause.insert(*(it->begin()));
        }
    }
    // check inconsistent
    for (set<int>::const_iterator it = unit_clause.begin();
            it != unit_clause.end(); ++ it) {
        if (unit_clause.find(- *it) != unit_clause.end()) {
            // inconsistent, return Lit
            return Lit(clauses);
        }
    }
    if (! unit_clause.empty()) {
        // return A \cup UP(assign(A, P))
        set<int> ret;
        set<int> assign_clauses = assign(unit_clause, clauses);
        set<int> tmp = UnitPropagation(assign_clauses);
        set_union(unit_clause.begin(), unit_clause.end(), tmp.begin(),
                tmp.end(), inserter(ret, ret.begin()));
        return ret;
    }
    return empty_set;
}

/*
 * 计算clauses w.r.t literals 的 unit propagation 结果
 */
set<int> Consequence::UnitPropagation(set<int> literals, set< set<int> > clauses) {
    set<int> conq;
    // UP(L, \Gamma) = L \cup UP( assign(L, \Gamma)). 
    set<int> temp = UnitPropagation(assign(literals, clauses));
    set_union(literals.begin(), literals.end(), temp.begin(), temp.end(),
            inserter(conq, conq.begin()));
    return conq;
}  


/*
 * 即UP(\Gamma)中的assgin方法
 */
set< set<int> > Consequence::assign(set<int> literals, set< set<int> > clauses) {
    set< set<int> > ret;
    // 计算_A
    set<int> neg_literals = literals;
    for (set<int>::iterator it = neg_literals.begin();
            it != neg_literals.end(); ++ it) {
        *it = -*it;
    }
    // {c | for some c_ in clauses, c_ ^ A = empty, c = c_ \ _A} 见吉老师KR08的论文
    for (set< set<int> >::const_iterator it = clauses.begin();
            it != clauses.end(); ++ it) {
        set<int> &cur_clause = *it;
        set<int> intersection;
        set_intersection(cur_clause.begin(), cur_clause.end(),
                literals.begin(), literals.end(),
                inserter(intersection, intersection.begin()));
        if (intersection.empty()) {
            set<int> diff;
            set_difference(cur_clause.begin(), cur_clause.end(),
                    neg_literals.begin(), neg_literals.end(),
                    inserter(diff, diff.begin()));
            if (! diff.empty()) {
                ret.insert(diff);
            }
        }
    }
    return ret;
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