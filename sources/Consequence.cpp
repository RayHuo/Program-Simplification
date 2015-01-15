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
 * Lit(P) = Atom(P) \cup {~a | a \in Atom(P)}
 */
set<int> Consequence::Lit() {
    set<int> ret;
    for (set<int>::iterator it = Atoms_P.begin();
            it != Atoms_P.end(); ++ it) {
        ret.insert(*it);
        ret.insert(- *it);
    }
    return ret;
}
/*
 * UnitPropagation without literals
 */
set<int> Consequence::UnitPropagation(set<set<int> > clauses) {
    const set<int> empty_set;
    if (clauses.find(empty_set) != clauses.end()) {
        // return Lit
        return Lit();
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
            return Lit();
        }
    }
    if (! unit_clause.empty()) {
        // return A \cup UP(assign(A, P))
        set<int> ret;
        set< set<int> > assign_clauses = assign(unit_clause, clauses);
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
    set<int> neg_literals;
    for (set<int>::iterator it = literals.begin();
            it != literals.end(); ++ it) {
        neg_literals.insert(- *it);
    }
    // {c | for some c_ in clauses, c_ ^ A = empty, c = c_ \ _A} 见吉老师KR08的论文
    for (set< set<int> >::iterator it = clauses.begin();
            it != clauses.end(); ++ it) {
        set<int> intersection;
        set_intersection(it->begin(), it->end(),
                literals.begin(), literals.end(),
                inserter(intersection, intersection.begin()));
        if (intersection.empty()) {
            set<int> diff;
            set_difference(it->begin(), it->end(),
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
set<int> Consequence::GUS(FILE* out, vector<Rule> P, set<int> L) {
    set<int> lfp = lfp_PhiL(out, P, L);
    set<int> result;
    // Atoms(P) \ lfp(\Phi_L(X) \ {p | p \in L})
    set_difference(Atoms_P.begin(), Atoms_P.begin(), lfp.begin(), lfp.end(), inserter(result, result.begin()));
    return result;
}


/*
 * \Phi_L(X)算子
 * \Phi_L(X) = {a | 存在r \in P, a \in head(r) 且 a \notin {p | ~p \in L},
 *                  body(r) \cap ({p | ~p \in L} \cup {~p | p \in L}) = \emptyset,
 *                  body^+(r) \subseteq (X \ {p | ~p \in L}), head(r) \cap L = \emptyset}
 */
set<int> Consequence::PhiL(FILE* out, vector<Rule> P, set<int> L, set<int> X) {
    set<int> phi;
    fprintf(out, "\nSatisfy rules :\n"); fflush(out);
    for(vector<Rule>::iterator pit = P.begin(); pit != P.end(); pit++) {
        set<int> head_L;
        set_intersection((pit->heads).begin(), (pit->heads).end(), L.begin(), L.end(), inserter(head_L, head_L.begin()));
        if(head_L.empty()) {    // 1. head(r) \cap L = \emptyset
            set<int> Ls;    // 对应 {p | ~p \in L} \cup {~p | p \in L}
            for(set<int>::iterator lit = L.begin(); lit != L.end(); lit++)
                Ls.insert((*lit)*(-1));
            set<int> body_L;
            set_intersection((pit->bodys).begin(), (pit->bodys).end(), Ls.begin(), Ls.end(), inserter(body_L, body_L.begin()));
            if(body_L.empty()) {    // 2. body(r) \cap ({p | ~p \in L} \cup {~p | p \in L}) = \emptyset
                set<int> NL;    // { p | ~p \in L }
                for(set<int>::iterator lit = L.begin(); lit != L.end(); lit++)
                    if(*lit < 0)
                        NL.insert((*lit)*(-1));
                set<int> X_NL;
                set_difference(X.begin(), X.end(), NL.begin(), NL.end(), inserter(X_NL, X_NL.begin()));
                set<int> pbody;
                for(set<int>::iterator bit = (pit->bodys).begin(); bit != (pit->bodys).end(); bit++)
                    if(*bit >= 0)
                        pbody.insert(*bit);
                // 3. body^+(r) \subseteq (X \ {p | ~p \in L})
                if(includes(X_NL.begin(), X_NL.end(), pbody.begin(), pbody.end())) {
                    (*pit).output(out); fflush(out);
                    // 4. a \in head(r)
                    for(set<int>::iterator hit = (pit->heads).begin(); hit != (pit->heads).end(); hit++) {
                        if(NL.find(*hit) == NL.end())   // 5. a \notin { p | ~p \in L }
                            phi.insert(*hit);
                    }
                }
                    
            }
        }
    }
    
    return phi;
}


/*
 * 计算 \Phi_L(X) \cup { p | p \in L} 的极小不动点。
 */
set<int> Consequence::lfp_PhiL(FILE* out, vector<Rule> P, set<int> L) {
    set<int> X;
    X.clear();
    set<int> Ls;
    for(set<int>::iterator it = L.begin(); it != L.end(); it++)
        if(*it >= 0)
            Ls.insert(*it);
    
    fprintf(out, "\nPhiL Start:\n\n");
    while(true) {    
        fprintf(out, "\nX : ");
        for(set<int>::iterator xit = X.begin(); xit != X.end(); xit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*xit));
            if(xit != --(X.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> phi = PhiL(out, P, L, X);   
        set<int> phi_L;
        set_union(phi.begin(), phi.end(), Ls.begin(), Ls.end(), inserter(phi_L, phi_L.begin()));
        fprintf(out, "\nPhi out :\n");
        for(set<int>::iterator pit = phi_L.begin(); pit != phi_L.end(); pit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*pit));
            if(pit != --(phi_L.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> X_phi_L;
        set_difference(X.begin(), X.end(), phi_L.begin(), phi_L.end(), inserter(X_phi_L, X_phi_L.begin()));
        if(X.size() == phi_L.size() && X_phi_L.empty())
            return X;
        X = phi_L;
    }
    
//    return X;
}


/*
 * WP(L) = UP(L, P) \cup ~GUS(P, L) 算子
 */
set<int> Consequence::W_P(FILE* out, set<int> L) {
    set<int> up = UnitPropagation(L, clauses);
    fprintf(out, "\nUP(L, P) : ");
    for(set<int>::iterator lit = up.begin(); lit != up.end(); lit++) {
        int id = *lit;
        if(id < 0) {
            fprintf(out, "~");
            id *= -1;
        }
        fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
        if(lit != --(up.end()))
            fprintf(out, ",");
    }
    fprintf(out, "\n");     fflush(out);
    
    
    set<int> gus = GUS(out, program, L);
    fprintf(out, "\nGUS : ");
    for(set<int>::iterator lit = gus.begin(); lit != gus.end(); lit++) {
        int id = *lit;
        if(id < 0) {
            fprintf(out, "~");
            id *= -1;
        }
        fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
        if(lit != --(gus.end()))
            fprintf(out, ",");
    }
    fprintf(out, "\n");     fflush(out);
    
    set<int> upAndgus;
    set_union(up.begin(), up.end(), gus.begin(), gus.end(), inserter(upAndgus, upAndgus.begin()));
    return upAndgus;
}


/*
 * 计算WP(L)的极小不动点。
 */
set<int> Consequence::lfp_WP(FILE* out) {
    set<int> wp;
    set<int> L;         
    L.clear();
    
    fprintf(out, "\nWP Start :\n");
    while(true) {
        fprintf(out, "\nL : ");
        for(set<int>::iterator lit = L.begin(); lit != L.end(); lit++) {
            int id = *lit;
            if(id < 0) {
                fprintf(out, "~");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(lit != --(L.end()))
                fprintf(out, ",");
        }
        fprintf(out, "\n");     fflush(out);
        
        wp = W_P(out, L);
        
        fprintf(out, "\nW_P(L) : ");
        for(set<int>::iterator sit = wp.begin(); sit != wp.end(); sit++) {
            int id = *sit;
            if(id < 0) {
                fprintf(out, "~");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(sit != --(wp.end()))
                fprintf(out, ",");
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> wp_L;
        set_difference(wp.begin(), wp.end(), L.begin(), L.end(), inserter(wp_L, wp_L.begin()));
        if(L.size() == wp.size() && wp_L.empty())
            return wp;
        L = wp;
    }
}


/*
 * 进行计算consequence中第4步的lookahead
 */
bool Consequence::Lookahead(set<int> L) {
    return true;
}


/*
 * 整合调用上面的函数来计算consequence
 */
set<int> Consequence::calConsequence() {
    set<int> literals;
    return UnitPropagation(literals, clauses);
}


/*
 * 输出clauses
 */
void Consequence::printClauses(FILE* out) {
    fprintf(out, "\nThe clauses :\n");
    for(set< set<int> >::iterator it = clauses.begin(); it != clauses.end(); it++) {
        fprintf(out, "{ ");
        for(set<int>::iterator sit = it->begin(); sit != it->end(); sit++) {
            int id = *sit;
            if(id < 0) {
                fprintf(out, "~");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(sit != --(it->end()))
                fprintf(out, ", ");
        }
        fprintf(out, " }\n");
    }
}