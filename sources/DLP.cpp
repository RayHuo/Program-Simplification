#include "DLP.h"
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
#include <cmath>

using namespace std;


/*
 * 默认构造函数
 */
DLP::DLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    NL.clear();
    LM.clear();
}


/*
 * 自定义构造函数
 */
DLP::DLP(vector<Rule> p, set<int> l) : program(p), L(l) {
    Atoms_P.clear();
    for(vector<Rule>::const_iterator pit = program.begin(); pit != program.end(); pit++) {
        Atoms_P.insert(pit->heads.begin(), pit->heads.end());   // 因为head中必然是非负的
        for(set<int>::const_iterator bit = pit->bodys.begin(); bit != pit->bodys.end(); bit++) {
            Atoms_P.insert(abs(*bit));
        }
    }
    
    NL.clear();
    LM.clear();
    for(set<int>::const_iterator lit = L.begin(); lit != L.end(); lit++) {
        if(*lit < 0) {
            NL.insert(*lit);
            LM.insert(abs(*lit));
        }
    }
}


/*
 * 析构函数
 */
DLP::~DLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    NL.clear();
    LM.clear();
}


/**
 * 论文中algorithm 1的函数的weak版本。
 * @param X   开始的X为空集
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSWPL(set<int> X, FILE* out) {
    fprintf(out, "\n+++++++++++++++++++++++++\nRSSPL :\n");
    fprintf(out, "X : ");
    for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
        if(*xit < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
    }
    fprintf(out, "\n");         fflush(out);
    
    fprintf(out, "L : ");
    for(set<int>::const_iterator xit = L.begin(); xit != L.end(); xit++) {
        if(*xit < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
    }
    fprintf(out, "\n");         fflush(out);
    
    fprintf(out,"H : \n");
    
    set<int> A = X;     // A := X
    set< set<int> > H;  // 算法中的H
    for(vector<Rule>::const_iterator r = program.begin(); r != program.end(); r++) {
        set<int> pb;    // body^+(r)
        for(set<int>::const_iterator bit = r->bodys.begin(); bit != r->bodys.end(); bit++) {
            if(*bit >= 0)
                pb.insert(*bit);
        }
        
        set<int> h_pb_intersection;     // head(r) \cap body^+(r)
        set_intersection(r->heads.begin(), r->heads.end(), pb.begin(), pb.end(), inserter(h_pb_intersection, h_pb_intersection.begin()));
        if(h_pb_intersection.empty()) {
            set<int> X_h_intersection;  // X \cap head(r)
            set_intersection(r->heads.begin(), r->heads.end(), X.begin(), X.end(), inserter(X_h_intersection, X_h_intersection.begin()));
            if(X_h_intersection.empty()) {
                set<int> XL;    // X \cup L
                set_union(X.begin(), X.end(), L.begin(), L.end(), inserter(XL, XL.begin()));
                if(includes(XL.begin(), XL.end(), r->bodys.begin(), r->bodys.end())) {
                    set<int> h_lm;      // head(r) \ L-
                    set_difference(r->heads.begin(), r->heads.end(), LM.begin(), LM.end(), inserter(h_lm, h_lm.begin()));
                    H.insert(h_lm);
                    
                    fprintf(out, "\trule : ");
                    r->output(out);
                    fprintf(out, "\t\titem : ");
                    for(set<int>::const_iterator hlmit = h_lm.begin(); hlmit != h_lm.end(); hlmit++) {
                        if(*hlmit < 0)
                            fprintf(out, "~");
                        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*hlmit)));
                    }
                    fprintf(out, "\n"); fflush(out);
                }
            }
        }
    }
    
    
    // for each C \in H
    for(set< set<int> >::const_iterator C= H.begin(); C != H.end(); C++) {
        if(C->size() == 1 || all_C_in_RSWPL(X, *C, out)) {
            set<int> A_C;       // A \cup C
            set_union(A.begin(), A.end(), C->begin(), C->end(), inserter(A_C, A_C.begin()));
            A = A_C;
        }
    }
    
    return A;
}
 
/**
 * 用于RSWPL中判断是否 for each p \in C, C \subseteq RSWPL(X \cup {p})
 * @param X
 * @param out
 * @return 
 */
bool DLP::all_C_in_RSWPL(set<int> X, set<int> C, FILE* out) {
    fprintf(out, "\nforeach p \\in C = { ");
    for(set<int>::const_iterator p = C.begin(); p != C.end(); p++) {
        if(*p < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*p)));
    }
    fprintf(out, "\n"); fflush(out);
    
    
    for(set<int>::const_iterator p = C.begin(); p != C.end(); p++) {
        set<int> p_set;  p_set.insert(*p);      // {p}
        set<int> XP;     // X \cup {p}
        set_union(X.begin(), X.end(), p_set.begin(), p_set.end(), inserter(XP, XP.begin()));
        set<int> rswpl = RSWPL(XP, out);
        
        
        fprintf(out, "p = %s, and rswpl(X \\cup {p}) = { ");
        for(set<int>::const_iterator it = rswpl.begin(); it != rswpl.end(); it++) {
            if(*it < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(out, "\n"); fflush(out);
        
        
        if(!includes(rswpl.begin(), rswpl.end(), C.begin(), C.end()))
            return false;
    }
    
    return true;
}
    


/**
 * 论文中algorithm 1的函数的strong版本。
 * @param X
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSSPL(set<int> X, FILE* out) {
    fprintf(out, "\n+++++++++++++++++++++++++\nRSSPL :\n");
    fprintf(out, "X : ");
    for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
        if(*xit < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
    }
    fprintf(out, "\n");         fflush(out);
    
    fprintf(out, "L : ");
    for(set<int>::const_iterator xit = L.begin(); xit != L.end(); xit++) {
        if(*xit < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
    }
    fprintf(out, "\n");         fflush(out);
    
    fprintf(out,"H : \n");
    
    
    set<int> A = X;     // A := X
    set< set<int> > H;  // 算法中的H
    for(vector<Rule>::const_iterator r = program.begin(); r != program.end(); r++) {
        set<int> pb;    // body^+(r)
        for(set<int>::const_iterator bit = r->bodys.begin(); bit != r->bodys.end(); bit++) {
            if(*bit >= 0)
                pb.insert(*bit);
        }
        
        set<int> h_pb_intersection;     // head(r) \cap body^+(r)
        set_intersection(r->heads.begin(), r->heads.end(), pb.begin(), pb.end(), inserter(h_pb_intersection, h_pb_intersection.begin()));
        if(h_pb_intersection.empty()) {
            set<int> X_h_intersection;  // X \cap head(r)
            set_intersection(r->heads.begin(), r->heads.end(), X.begin(), X.end(), inserter(X_h_intersection, X_h_intersection.begin()));
            if(X_h_intersection.empty()) {
                set<int> XNL;    // X \cup (L \ L+)
                set_union(X.begin(), X.end(), NL.begin(), NL.end(), inserter(XNL, XNL.begin()));
                if(includes(XNL.begin(), XNL.end(), r->bodys.begin(), r->bodys.end())) {
                    set<int> h_lm;      // head(r) \ L-
                    set_difference(r->heads.begin(), r->heads.end(), LM.begin(), LM.end(), inserter(h_lm, h_lm.begin()));
                    H.insert(h_lm);
                    
                    fprintf(out, "\trule : ");
                    r->output(out);
                    fprintf(out, "\t\titem : ");
                    for(set<int>::const_iterator hlmit = h_lm.begin(); hlmit != h_lm.end(); hlmit++) {
                        if(*hlmit < 0)
                            fprintf(out, "~");
                        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*hlmit)));
                    }
                    fprintf(out, "\n"); fflush(out);
                }
            }
        }
    }
    
    
    // for each C \in H
    for(set< set<int> >::const_iterator C= H.begin(); C != H.end(); C++) {
        if(C->size() == 1 || all_C_in_RSSPL(X, *C, out)) {
            set<int> A_C;       // A \cup C
            set_union(A.begin(), A.end(), C->begin(), C->end(), inserter(A_C, A_C.begin()));
            A = A_C;
        }
    }
    
    return A;
}

/**
 * 用于RSWPL中判断是否 for each p \in C, C \subseteq RSSPL(X \cup {p})
 * @param X
 * @param out
 * @return 
 */
bool DLP::all_C_in_RSSPL(set<int> X, set<int> C, FILE* out) {
    fprintf(out, "\nforeach p \\in C = { ");
    for(set<int>::const_iterator p = C.begin(); p != C.end(); p++) {
        if(*p < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*p)));
    }
    fprintf(out, "\n"); fflush(out);
    
    
    for(set<int>::const_iterator p = C.begin(); p != C.end(); p++) {
        set<int> p_set;  p_set.insert(*p);      // {p}
        set<int> XP;     // X \cup {p}
        set_union(X.begin(), X.end(), p_set.begin(), p_set.end(), inserter(XP, XP.begin()));
        set<int> rsspl = RSSPL(XP, out);
        
        
        fprintf(out, "p = %s, and rsspl(X \\cup {p}) = { ");
        for(set<int>::const_iterator it = rsspl.begin(); it != rsspl.end(); it++) {
            if(*it < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fprintf(out, "\n"); fflush(out);
        
        
        if(!includes(rsspl.begin(), rsspl.end(), C.begin(), C.end()))
            return false;
    }
    
    return true;
} 


/**
 * 计算RSWPL的极小不动点
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSW(FILE* out) {
    set<int> X; 
    X.clear();          // 计算极小不动点，一开始从空集开始
    while(true) {
        fprintf(out, "\n---------------------------\nRSW :\n");
        fprintf(out, "\nRSW X : ");
        for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> rswpl = RSWPL(X, out);
        
        fprintf(out, "\nRSW rswpl : ");
        for(set<int>::const_iterator xit = rswpl.begin(); xit != rswpl.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        if(rswpl.size() == X.size() && includes(rswpl.begin(), rswpl.end(), X.begin(), X.end()))
            return rswpl;
        
        X = rswpl;
    }
}


/**
 * 计算RSSPL的极小不动点
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSS(FILE* out) {
    set<int> X; 
    X.clear();          // 计算极小不动点，一开始从空集开始
    while(true) {
        fprintf(out, "\n---------------------------\nRSS :\n");
        fprintf(out, "\nRSS X : ");
        for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> rsspl = RSSPL(X, out);
        
        fprintf(out, "\nRSS rsspl : ");
        for(set<int>::const_iterator xit = rsspl.begin(); xit != rsspl.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        if(rsspl.size() == X.size() && includes(rsspl.begin(), rsspl.end(), X.begin(), X.end()))
            return rsspl;
        
        X = rsspl;
    }
}


/**
 * 直接调用RWS得到结果
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::GWRS(FILE* out) {
    fprintf(out, "\n=============================\nStart DLP GWRS : \n");
    set<int> gwrs = RSW(out);
    return gwrs;
}


/**
 * 直接调用RSS得到结果
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::GSRS(FILE* out) {
    fprintf(out, "\n=============================\nStart DLP GSRS : \n");
    set<int> gsrs = RSS(out);
    return gsrs;
}