#include "MaxU.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

extern vector<Rule> G_Rules;

/*
 * 默认构造函数
 */
MaxU::MaxU() {
    p_quote.clear();
    U.clear();
    E.clear();
    p_sharp.clear();
}

/*
 * 使用指定的rule集合来初始化p_quote
 */
MaxU::MaxU(vector<Rule> p) : p_quote(p) {
    U.clear();
    E.clear();
    p_sharp.clear();
}

/*
 * 析构函数
 */
MaxU::~MaxU() {
    p_quote.clear();
    U.clear();
    E.clear();
    p_sharp.clear();
}
    
/*
 * 判断P'中是否还有事实。
 */
bool MaxU::containFact() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if(it->type == FACT && (it->heads).size() == 1)
            return false;
    }
    return true;
}

/*
 * 把P'中的事实放进U中来初始化U。
 * 仅把头部只含一个原子的事实放进U中。
 */
void MaxU::initializeU() {
    U.clear();
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if(it->type == FACT && (it->heads).size() == 1) {
            U.insert(*((it->heads).begin()));
        }
    }
}
   
/*
 * 即tr_p(P, U)，这里直接使用program与U作为参数，所以函数没有形参。
 * 化简的规则是，对于program中的rule：
 *      头部、负体中出现U中元素的删除该规则，
 *      正体中出现U中元素的删除该原子。
 * 所以这里的实际操作是：判断每条rule
 *      抽取rule的负体和正体;
 *      判断U与头部、负体和正体是否有交集，并执行相关的删除操作
 */
void MaxU::tr_p() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); ) {
        // 提取负体和正体
        set<int> pBody;
        set<int> nBody;
        for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
            if(*bit >= 0)
                pBody.insert(*bit);
            else
                nBody.insert(*bit);
        }
        
        set<int> h_inter;       // 头部与U的交集
        set<int> nb_inter;      // 负体与U的交集
        set<int> pb_inter;      // 正体与U的交集
        set_intersection(U.begin(), U.end(), (it->heads).begin(), (it->heads).end(), inserter(h_inter, h_inter.begin()));
        set_intersection(U.begin(), U.end(), nBody.begin(), nBody.end(), inserter(nb_inter, nb_inter.begin()));
        set_intersection(U.begin(), U.end(), pBody.begin(), pBody.end(), inserter(pb_inter, pb_inter.begin()));
        
        if(!h_inter.empty() || !nb_inter.empty()) {
            it = p_quote.erase(it);
        }
        else if(!pb_inter.empty()) {
            set<int> diff;
            set_difference((it->bodys).begin(), (it->bodys).end(), U.begin(), U.end(), inserter(diff, diff.begin()));
            it->bodys = diff;
            it++;
        }
        else {
            it++;
        }
    }
}
    
/*
 * 循环算法步骤2~4，直到把P'完全化简
 */
void MaxU::simplifyProgram() {
    while(containFact()) {
        initializeU();
        tr_p();
    }
}
    
/*
 * 算法步骤5中，把当前程序的析取事实放进E中，
 * 注意：只需要把集合意义下最小的析取事实放进去就可以了。
 */
void MaxU::updateE() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if((it->type) == FACT && (it->heads).size() > 1) {
            // 只把集合意义下最小的放进去
            bool flag = false;
            for(set< set<int> >::iterator eit = E.begin(); eit != E.end(); eit++) {
                if(includes((it->heads).begin(), (it->heads).end(), (*eit).begin(), (*eit).end())) {
                    flag = true;
                    break;
                }
            }
            if(flag)
                continue;
            E.insert(it->heads);
        }
    }
}
 
/*
 * 从P'中根据C提取出P*
 */
void MaxU::extractPSharp(set<int> C) {
    p_sharp.clear();
    
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        // 判断是否正程序
        bool isPositive = true;
        for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
            if(*bit < 0) {
                isPositive = false;
                break;
            }
        }
        if(!isPositive)
            continue;
            
        // 判断是否Atoms(r)是否为C的子集
        set<int> atoms_r;
        set_union((it->heads).begin(), (it->heads).end(), (it->bodys).begin(), (it->bodys).end(),
                inserter(atoms_r, atoms_r.begin()));
        if(includes(C.begin(), C.end(), atoms_r.begin(), atoms_r.end())) {
            p_sharp.insert(*it);
        }    
    }
}
  

/*
 * 判断P*中是否存在满足要求的Loop
 * 如果存在，返回该Loop，
 * 如果不存在，则返回一个空的Loop
 */   
Loop MaxU::existLoop(set<int> C) {
    Loop result;
    
    DependenceGraph dpdg(p_sharp);
    dpdg.findSCCs();
    vector<Loop> SCCs = dpdg.getSCCs();
    
    bool exist = false; 
    int i;
    // 因为一边循环中涉及对SCCs的插入操作，所以使用下标进行遍历
    for(i = 0; i < SCCs.size(); i++) {
        Loop S = SCCs.at(i);
        set<int> es_rules = SCCs.at(i).ESRules;
        
        // 如果isSubset为true，即当前外部支持的head(r)是L的子集，即当前Loop不是满足要求的环，继续判断下一个环。
        bool isSubset = false;
        for(set<int>::iterator esit = es_rules.begin(); esit != es_rules.end(); esit++) {
            Rule rule = G_Rules.at(*esit - 1);
            // 如果当前满足head(r)是L的子集，则当前这个Loop不符合要求，不用再判断后面的外部支持了，进而执行8.3.2。
            if(includes(S.loopNodes.begin(), S.loopNodes.end(), 
                    rule.heads.begin(), rule.heads.end())) {
                isSubset = true;
                break;
            }
        }
        
        // 如果isSubset为true，即当前Loop不满足要求，执行8.3.2，即把其所有下一级子环加入到SCCs中，并继续判断下一个环。
        if(isSubset) {
            // 对原图分别减去每一个原子得到的图找SCC，并加入到SCCs中，然后继续循环SCCs。
            // 注意：这里找到SCC后，需要更新其外部支持，所以可以考虑把找一个环的外部支持的函数抽象出来。
            for(set<int>::iterator lit = S.loopNodes.begin(); lit != S.loopNodes.end(); lit++) {
                set<int> induceAtoms;
                induceAtoms.insert(*lit);
                DependenceGraph subgraph(p_sharp);
                map<int, set<int> > induceGraph = subgraph.induceSubgraph(induceAtoms);
                subgraph.resetDpdGraph(induceGraph);
                subgraph.findSCCs();
                vector<Loop> newSCCs = subgraph.getSCCs();
                SCCs.insert(SCCs.end(), newSCCs.begin(), newSCCs.end());                               
            }
            // 执行完这个更新SCCs的循环后，就直接遍历下一个SCC了。
        }
        
        // 如果isSubset为false，即当前Loop满足要求，所以“存在这样的一个loop，返回步骤6”。
        else {
            exist = true;
            break;
        }
    }
    
    // 在遍历完所有loop或者找到了一个满足条件的loop时，
    // 如果是后者：exist为false，即“存在这样的一个loop，返回步骤6”。
    if(!exist) {
        result = SCCs.at(i);    // 设置好result，然后返回。
    }
    // 如果是前者：exist为true，即“不存在这样的一个loop，执行步骤10”。
    else {
        U.insert(C.begin(), C.end());
        // 不对result进行操作，后续直接返回一个空的Loop
    }
    
    return result;
}


/*
 * 就算U的整个过程，调用上面的各个函数，主要是实现步骤5～11这个过程。
 */
void MaxU::calU() {
    
}
