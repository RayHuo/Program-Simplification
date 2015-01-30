#include "GRSNLP.h"
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
GRSNLP::GRSNLP() {
    program.clear();
    ATOMS_P.clear();
}


/*
 * 自定义构造函数
 */
GRSNLP::GRSNLP(vector<Rule> P) : program(P) {
    ATOMS_P.clear();
    for (vector<Rule>::const_iterator rule_it = program.begin();
            rule_it != program.end(); ++ rule_it) {
        for (set<int>::const_iterator head_it = rule_it->heads.begin();
                head_it != rule_it->heads.end(); ++ head_it) {
            ATOMS_P.insert(abs(*head_it));
        }
        for (set<int>::const_iterator body_it = rule_it->bodys.begin();
                body_it != rule_it->bodys.end(); ++ body_it) {
            ATOMS_P.insert(abs(*body_it));
        }
    }
}


/*
 * 析构函数
 */
GRSNLP::~GRSNLP() {
    program.clear();
    ATOMS_P.clear();
}


/*
 * algorithm1中的uc_P(L)函数 
 */
set<int> GRSNLP::UCP(set<int> L) {
    set<int> X = ATOMS_P;
    set<int> E;
    set<int> tmp;
    do {
        // E = { head(r) | r属于R-(X,P）such that L满足body(r) }
        E.clear();
        for (vector<Rule>::const_iterator rule_it = program.begin();
                rule_it != program.end(); ++ rule_it) {
            // head(r) \cap X != empty
            tmp.clear();
            set_intersection(rule_it->heads.begin(), rule_it->heads.end(),
                    X.begin(), X.end(), inserter(tmp, tmp.begin()));
            if (tmp.empty()) {
                continue;
            }
            // X \cap body+(r) == empty
            set<int> body_plus;
            for (set<int>::const_iterator body_it = rule_it->bodys.begin();
                    body_it != rule_it->bodys.end(); ++ body_it) {
                if (*body_it > 0) {
                    body_plus.insert(*body_it);
                }
            }
            tmp.clear();
            set_intersection(X.begin(), X.end(), body_plus.begin(),
                    body_plus.end(), inserter(tmp, tmp.begin()));
            if (! tmp.empty()) {
                continue;
            }
            // 该规则属于外部支持, 检查L是否满足body(r)即L是否包含body(r)
            if (includes(L.begin(), L.end(), rule_it->bodys.begin(), 
                    rule_it->bodys.end())) {
                E.insert(rule_it->heads.begin(), rule_it->heads.end());
            }
        }
        tmp.clear();
        set_difference(X.begin(), X.end(), E.begin(), E.end(),
                inserter(tmp, tmp.begin()));
        X = tmp;
    } while (! E.empty());
    return X;
}


/*
 * 
 */
set<int> GRSNLP::calGRS(FILE *out) {
    Consequence consequence(program);
    set<int> L = consequence.calConsequence(out);
//    set<int> L; L.insert(1); L.insert(2);
    fprintf(out, "\nFinal Consequence : ");
    for(set<int>::iterator cit = L.begin(); cit != L.end(); cit++) {
        int id = *cit;
            if(id < 0) {
                fprintf(out, "~");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(cit != --(L.end()))
                fprintf(out, ", ");
    }
    fprintf(out, "\n");        fflush(out);
    
    set<int> X;
    // X = ATOM(P) \ {p | ~p in L}
    for (set<int>::const_iterator it = ATOMS_P.begin();
            it != ATOMS_P.end(); ++ it) {
        if (L.find(- *it) != L.end()) {
            continue;
        }
        X.insert(*it);
    }
    fprintf(out, "\n------------------------\nRS Start:\n\n");
    while (true) {
        set<int> X_L;
        set_union(X.begin(), X.end(), L.begin(), L.end(),
                inserter(X_L, X_L.begin()));
        
        fprintf(out, "\nX : ");
        for(set<int>::iterator xit = X.begin(); xit != X.end(); xit++) {
            if (*xit < 0) {
                fprintf(out, "~");
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(abs(*xit)));
            if(xit != --(X.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        fprintf(out, "\nX \\cap L : ");
        for(set<int>::iterator xit = X_L.begin(); xit != X_L.end(); xit++) {
            if (*xit < 0) {
                fprintf(out, "~");
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(abs(*xit)));
            if(xit != --(X_L.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> UC_P = UCP(X_L);
        fprintf(out, "\nUC(X \\cap L) : ");
        for(set<int>::iterator it = UC_P.begin(); it != UC_P.end(); it++) {
            if (*it < 0) {
                fprintf(out, "~");
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(abs(*it)));
            if(it != --(UC_P.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);

        set<int> RS;
        set_difference(ATOMS_P.begin(), ATOMS_P.end(), UC_P.begin(), UC_P.end(),
                inserter(RS, RS.begin()));
        fprintf(out, "\nRS : ");
        for(set<int>::iterator it = RS.begin(); it != RS.end(); it++) {
            if (*it < 0) {
                fprintf(out, "~");
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(abs(*it)));
            if(it != --(RS.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        if (RS == X) {
            break;
        }
        X = RS;
    }
    
    return X;
}