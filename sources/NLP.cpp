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
#include <algorithm>
#include <cmath>

using namespace std;


/*
 * 默认构造函数
 */
NLP::NLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    NL.clear();
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
    
    NL.clear();
    for(set<int>::const_iterator lit = L.begin(); lit != L.end(); lit++) {
        if(*lit < 0)
            NL.insert(*lit);
    }
}


/*
 * 析构函数
 */
NLP::~NLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    NL.clear();
}


/*
 * 论文中的T^w_P,L(X)函数。
 * 这里边考虑使用一下STL中的bind方法，让一些功能变得简单
 */
set<int> NLP::TWPL(set<int> X, FILE* out) {
    set<int> result;
    for(vector<Rule>::const_iterator r = program.begin(); r != program.end(); r++) {
        set<int> pb;    // body^+(r)
        for(set<int>::const_iterator it = r->bodys.begin(); it != r->bodys.end(); it++) {
            if(*it >= 0)
                pb.insert(*it);
        }
        
        set<int> h_pb;  // head(r) \ body^+(r)
        set_difference(r->heads.begin(), r->heads.end(), pb.begin(), pb.end(), inserter(h_pb, h_pb.begin()));
        
        
        fprintf(out, "\n++++++++++++++++++\nhead(r) \\ body^+(r) : ");
        for(set<int>::const_iterator xit = h_pb.begin(); xit != h_pb.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        
        // 遍历每一个满足 p \in head(r) 且 p \notin body^+(r) 的原子p
        for(set<int>::const_iterator p = h_pb.begin(); p != h_pb.end(); p++) {
            set<int> LX;        // L \cup X
            set_union(L.begin(), L.end(), X.begin(), X.end(), inserter(LX, LX.begin()));
            
            fprintf(out, "\nL \\cup X : ");
            for(set<int>::const_iterator xit = LX.begin(); xit != LX.end(); xit++) {
                if(*xit < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
            }
            fprintf(out, "\n");     fflush(out);
            
            set<int> p_set;     p_set.insert(*p);       // 把 p 弄成 {p}
            set<int> h_pset;    // head(r) \ {p}
            set_difference(r->heads.begin(), r->heads.end(), p_set.begin(), p_set.end(), inserter(h_pset, h_pset.begin()));
            set<int> qs;        // {~q | q \in head(r) \ {p}}，集合关系为合取
            for(set<int>::const_iterator hpit = h_pset.begin(); hpit != h_pset.end(); hpit++)
                qs.insert(*hpit * -1);
            
            set<int> b_qs;      // body(r) \cup qs
            set_union(r->bodys.begin(), r->bodys.end(), qs.begin(), qs.end(), inserter(b_qs, b_qs.end()));
            
            fprintf(out, "\nbody(r) \\cup qs : ");
            for(set<int>::const_iterator xit = b_qs.begin(); xit != b_qs.end(); xit++) {
                if(*xit < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
            }
            fprintf(out, "\n");     fflush(out);
            
            
            // 判断是否满足 L \cup X |= body(r) \cup qs，这个可以直接使用集合包含关系判断
            if(includes(LX.begin(), LX.end(), b_qs.begin(), b_qs.end()))
                result.insert(*p);
        }
    }
    
    return result;
}
   

/*
 * 论文中的T^s_P,L(X)函数。
 */
set<int> NLP::TSPL(set<int> X, FILE* out) {
    set<int> result;
    for(vector<Rule>::const_iterator r = program.begin(); r != program.end(); r++) {
        set<int> pb;    // body^+(r)
        for(set<int>::const_iterator it = r->bodys.begin(); it != r->bodys.end(); it++) {
            if(*it >= 0)
                pb.insert(*it);
        }
        
        set<int> h_pb;  // head(r) \ body^+(r)
        set_difference(r->heads.begin(), r->heads.end(), pb.begin(), pb.end(), inserter(h_pb, h_pb.begin()));
        
        
        fprintf(out, "\n++++++++++++++++++\nhead(r) \\ body^+(r) : ");
        for(set<int>::const_iterator xit = h_pb.begin(); xit != h_pb.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        
        
        // 遍历每一个满足 p \in head(r) 且 p \notin body^+(r) 的原子p
        for(set<int>::const_iterator p = h_pb.begin(); p != h_pb.end(); p++) {
//            set<int> NL;         
//            for(set<int>::const_iterator lit = L.begin(); lit != L.end(); lit++) {
//                if(*lit < 0)
//                    NL.insert(*lit);
//            }
            
            set<int> NLX;        // (L \ L^+) \cup X，下面的NL，为L \ L^+，其中 L^+ = { p | p \in L }
            set_union(NL.begin(), NL.end(), X.begin(), X.end(), inserter(NLX, NLX.begin()));
            
            
            fprintf(out, "\n(L \\ L^+) \\cup X : ");
            for(set<int>::const_iterator xit = NLX.begin(); xit != NLX.end(); xit++) {
                if(*xit < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
            }
            fprintf(out, "\n");     fflush(out);
            
            
            set<int> p_set;     p_set.insert(*p);       // 把 p 弄成 {p}
            set<int> h_pset;    // head(r) \ {p}
            set_difference(r->heads.begin(), r->heads.end(), p_set.begin(), p_set.end(), inserter(h_pset, h_pset.begin()));
            set<int> qs;        // {~q | q \in head(r) \ {p}}，集合关系为合取
            for(set<int>::const_iterator hpit = h_pset.begin(); hpit != h_pset.end(); hpit++)
                qs.insert(*hpit * -1);
            
            set<int> b_qs;      // body(r) \cup qs
            set_union(r->bodys.begin(), r->bodys.end(), qs.begin(), qs.end(), inserter(b_qs, b_qs.end()));
            
            fprintf(out, "\nbody(r) \\cup qs : ");
            for(set<int>::const_iterator xit = b_qs.begin(); xit != b_qs.end(); xit++) {
                if(*xit < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
            }
            fprintf(out, "\n");     fflush(out);
            
            
            // 判断是否满足 (L \ L^+) \cup X |= body(r) \cup qs，这个可以直接使用集合包含关系判断
            if(includes(NLX.begin(), NLX.end(), b_qs.begin(), b_qs.end()))
                result.insert(*p);
        }
    }
    
    return result;
}
   

/*
 * 论文中的lfp(T^w_P,L(X)) = GWRS(P,L)，这里本应该是有参数P和L的，但实际是直接使用program和L这两个属性
 */
set<int> NLP::TW(FILE* out) {
    set<int> X;    X.clear();  // 计算极小不动点，故X从空集开始
    
    while(true) {
        fprintf(out, "\n---------------------------\nTW :\n");
        fprintf(out, "\nTW X : ");
        for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> twpl = TWPL(X, out);
        
        fprintf(out, "\nTW twpl : ");
        for(set<int>::const_iterator xit = twpl.begin(); xit != twpl.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        if(twpl.size() == X.size() && includes(twpl.begin(), twpl.end(), X.begin(), X.end()))
            return twpl;
        
        X = twpl;
    }
}


/*
 * 论文中的lfp(T^s_P,L(X)) = GSRS(P,L)，这里本应该是有参数P和L的，但实际是直接使用program和L这两个属性
 */
set<int> NLP::TS(FILE* out) {
    set<int> X;    X.clear();  // 计算极小不动点，故X从空集开始
    
    while(true) {
        fprintf(out, "\n---------------------------\nTS :\n");
        fprintf(out, "\nTS X : ");
        for(set<int>::const_iterator xit = X.begin(); xit != X.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        set<int> tspl = TSPL(X, out);
        
        fprintf(out, "\nTS tspl : ");
        for(set<int>::const_iterator xit = tspl.begin(); xit != tspl.end(); xit++) {
            if(*xit < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*xit)));
        }
        fprintf(out, "\n");     fflush(out);
        
        if(tspl.size() == X.size() && includes(tspl.begin(), tspl.end(), X.begin(), X.end()))
            return tspl;
        
        X = tspl;
    }
}
  

/*
 * 直接调用TW()
 */
set<int> NLP::GWRS(FILE* out) {
    fprintf(out, "\n=============================\nStart NLP GWRS : \n");
    set<int> gwrs = TW(out);
    return gwrs;
}
 

/*
 * 直接调用TS()
 */
set<int> NLP::GSRS(FILE* out) {
    fprintf(out, "\n=============================\nStart NLP GSRS : \n");
    set<int> gsrs = TS(out);
    return gsrs;
}