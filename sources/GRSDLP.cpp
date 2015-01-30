#include "GRSDLP.h"
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
 * 构造函数
 */
GRSDLP::GRSDLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    num = 0;
}
 
/*
 * 自定义构造函数
 */
GRSDLP::GRSDLP(vector<Rule> p, set<int> l) : program(p), L(l){
    Atoms_P.clear();
    for(vector<Rule>::const_iterator pit = program.begin(); pit != program.end(); pit++) {
        Atoms_P.insert(pit->heads.begin(), pit->heads.end());   // 因为head中必然是非负的
        for(set<int>::const_iterator bit = pit->bodys.begin(); bit != pit->bodys.end(); bit++) {
            Atoms_P.insert(abs(*bit));
        }
    }
    num = 0;
}
 
/*
 * 析构函数
 */
GRSDLP::~GRSDLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
    num = -1;
}
    
/*
 * 实现论文中的algorithm2
 */   
set<int> GRSDLP::UCLP(set<int> U, set<int> X, FILE* out) {
//    if(num++ > 40000) {          // 在50000时就会segement fault，可能是爆内存了，毕竟递归得太深了。
//        set<int> empty;
//        return empty;
//    }
//    fprintf(out, "X in : %d.  ", X.size());     fflush(out);    // 某一步传入的X突然跪了
    
    // 准备工作
    set< set<int> > E;
    set<int> UL;        // U \cup L
    set<int> NL;        // {p | ~p \in L}
    
    set_union(U.begin(), U.end(), L.begin(), L.end(), inserter(UL, UL.begin()));
    
    for(set<int>::const_iterator lit = L.begin(); lit != L.end(); lit++) {
        if(*lit < 0)
            NL.insert(abs(*lit));
    }
    
    // 输出pre info
    fprintf(out, "\n\nUCLP : \n");
    fprintf(out, "U : ");
    for(set<int>::const_iterator it = U.begin(); it != U.end(); it++) {
        if(*it < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
    }
    fprintf(out, "\nX : ");
    for(set<int>::const_iterator it = X.begin(); it != X.end(); it++) {
        if(*it < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
    }
    fprintf(out, "\nL : ");
    for(set<int>::const_iterator it = L.begin(); it != L.end(); it++) {
        if(*it < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
    }
    fprintf(out, "\nU \\cup L : ");
    for(set<int>::const_iterator it = UL.begin(); it != UL.end(); it++) {
        if(*it < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
    }
    fprintf(out, "\n{p | ~p \\in L} : ");
    for(set<int>::const_iterator it = NL.begin(); it != NL.end(); it++) {
        if(*it < 0)
            fprintf(out, "~");
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
    }
    fprintf(out, "\n"); fflush(out);
    
    // 开始算法
    while(true) {
        // 计算得到 E
        E.clear();
        vector<Rule> esRule_XP = Utils::findESRules(program, X);
        
        fprintf(out, "ES rules : \n");
        for(vector<Rule>::const_iterator it = esRule_XP.begin(); it != esRule_XP.end(); it++) {
            fprintf(out, "\t");
            it->output(out);
        }
        fprintf(out, "\n");     fflush(out);
        
        for(vector<Rule>::const_iterator esit = esRule_XP.begin(); esit != esRule_XP.end(); esit++) {
            // 判断是否满足：U \cup L |= body(r)
            if(includes(UL.begin(), UL.end(), (esit->bodys).begin(), (esit->bodys).end())) {
                // 判断是否，满足：head(r) \ {p | ~p \in L} \subset X
                set<int> hL;
                set_difference((esit->heads).begin(), (esit->heads).end(), NL.begin(), NL.end(), inserter(hL, hL.begin()));
                if(includes(X.begin(), X.end(), hL.begin(), hL.end())) {
//                    E.insert(esit->heads);
                    E.insert(hL);
                }
            }
        }
        
        fprintf(out, "E : \n");
        for(set< set<int> >::const_iterator it = E.begin(); it != E.end(); it++) {
            fprintf(out, "\t{ ");
            for(set<int>::const_iterator tit = it->begin(); tit != it->end(); tit++) {
                if(*tit < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*tit)));
            }
            fprintf(out, "}\n");
        }
        fprintf(out, "\n");
        
        set<int> XHold = X;     // 保留着当前的X，以用于后面判断是否结束循环
        fprintf(out, "\nXHold : ");
        for(set<int>::const_iterator it = XHold.begin(); it != XHold.end(); it++) {
            if(*it < 0)
                fprintf(out, "~");
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
        }
        fflush(out);
        
        // 遍历 E 中的每个元素 C
        for(set< set<int> >::const_iterator C = E.begin(); C != E.end(); C++) {
            if(C->size() == 1) {
                // X = X \ C
                set<int> XC;
                set_difference(X.begin(), X.end(), C->begin(), C->end(), inserter(XC, XC.begin()));
                X = XC;
            }
            else {
                // Y = \bigcup_{p \in C}(C \cap uclp(U, X \ {p}))
                set<int> Y;     Y.clear();
                for(set<int>::const_iterator cit = C->begin(); cit != C->end(); cit++) {
                    set<int> X_p;       X_p.clear();
//                    X_p.erase(X_p.find(*cit));  // X \ {p}
                    set<int> p; p.insert(*cit);
                    set_difference(X.begin(), X.end(), p.begin(), p.end(), inserter(X_p, X_p.begin()));
                    set<int> uclp = UCLP(U, X_p, out);  // 递归调用
                    set<int> c_uclp_intersection;
                    set_intersection(C->begin(), C->end(), uclp.begin(), uclp.end(), inserter(c_uclp_intersection, c_uclp_intersection.begin()));
                    set<int> Ys;    Ys.clear();
                    set_union(Y.begin(), Y.end(), c_uclp_intersection.begin(), c_uclp_intersection.end(), inserter(Ys, Ys.begin()));
                    Y = Ys;
                }
                
                fprintf(out, "\nY : ");
                for(set<int>::const_iterator it = Y.begin(); it != Y.end(); it++) {
                    if(*it < 0)
                        fprintf(out, "~");
                    fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
                }
                fflush(out);
              
                // X = X \ (C \ Y)
                set<int> C_Y;   C_Y.clear();   // C \ Y
                set_difference(C->begin(), C->end(), Y.begin(), Y.end(), inserter(C_Y, C_Y.begin()));
                set<int> X_C_Y;         X_C_Y.clear();
                set_difference(X.begin(), X.end(), C_Y.begin(), C_Y.end(), inserter(X_C_Y, X_C_Y.begin()));
                X = X_C_Y;
                
            }
            
            fprintf(out, "\nX : ");
            for(set<int>::const_iterator it = X.begin(); it != X.end(); it++) {
                if(*it < 0)
                    fprintf(out, "~");
                fprintf(out, "%s ", Vocabulary::instance().getAtomName(abs(*it)));
            }
            fprintf(out, "\n");     fflush(out);
            
        }
        
        // 用于判断X是否没有变化，如果是则结束循环
        set<int> diff;
        set_difference(XHold.begin(), XHold.end(), X.begin(), X.end(), inserter(diff, diff.begin()));
        if(diff.empty() && XHold.size() == X.size())
            break;
    }
    
    return X;
}


/*
 * 计算RSLP(U)的函数
 */
set<int> GRSDLP::RSLP(set<int> U, FILE* out) {
    set<int> rslp;
    set<int> uclp = UCLP(U, Atoms_P, out);
    set_difference(Atoms_P.begin(), Atoms_P.end(), uclp.begin(), uclp.end(), inserter(rslp, rslp.begin()));
    return rslp;
}


/*
 * 计算gfp(RSLP(U))的函数，实际计算结果就是DLP的greatest reliable set
 */
set<int> GRSDLP::gfp_RSLP(FILE* out) {
    set<int> U = Atoms_P;
    while(true) {
        fprintf(out, "\n\nworking-------------------------");
        set<int> output = RSLP(U, out);
        
        // 判断U是否与输出output相等，若是则计算得到极大不动点，返回结果
        if(U.size() == output.size() &&
                includes(U.begin(), U.end(), output.begin(), output.end()))
            return output;
        
        // 否则继续迭代
        U = output;
    }
}


/*
 * 通过计算极大不动点来求得DLP的greatest reliable set的过程
 */
set<int> GRSDLP::calGRSDLP(FILE* out) {
    fprintf(out, "\n\n============================================================\nStart GRSDLP : \n\n");
    set<int> grs = gfp_RSLP(out);
    printf("\nget GRSDLP\n");
    return grs;
}
