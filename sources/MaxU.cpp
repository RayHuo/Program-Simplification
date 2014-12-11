#include "MaxU.h"
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

extern vector<Rule> G_Rules;

/*
 * 默认构造函数
 */
MaxU::MaxU() {
    p_quote.clear();
    U.clear();
    p_sharp.clear();
}

/*
 * 使用指定的rule集合来初始化p_quote
 */
MaxU::MaxU(vector<Rule> p) : p_quote(p) {
    U.clear();
    p_sharp.clear();
}

/*
 * 析构函数
 */
MaxU::~MaxU() {
    p_quote.clear();
    U.clear();
    p_sharp.clear();
}
    
/*
 * 判断P'中是否还有事实。
 * 此处的事实仅考虑头部只有一个原子的事实，即不包含析取事实
 */
bool MaxU::containFact() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if(it->type == FACT && (it->heads).size() == 1)
            return true;
    }
    return false;
}

/*
 * 把P'中的事实放进U中来初始化U。
 * 仅把头部只含一个原子的事实放进U中。
 */
void MaxU::initializeU() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if(it->type == FACT && (it->heads).size() == 1) {
            U.insert(*((it->heads).begin()));
        }
    }
}

    
/*
 * 循环算法步骤2~4，直到把P'完全化简
 */
void MaxU::simplifyProgram(FILE* out) {
    U.clear();
    while(containFact()) {
        initializeU();
        
        // 输出U
        fprintf(out, "\n------------------\nU : ");
        for(set<int>::iterator uit = U.begin(); uit != U.end(); uit++)
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(*uit));
        fprintf(out, "\n");
        
        Utils::tr_p(p_quote, U);
        
        // 输出P’
        fprintf(out, "\nP' is : \n");
        for(vector<Rule>::iterator pit = p_quote.begin(); pit != p_quote.end(); pit++) {
            pit->output(out);
        }
        fprintf(out, "----------------------\n");
    }
}
    
/*
 * 算法步骤5中，把当前程序的析取事实放进E中，
 * 注意：只需要把集合意义下最小的析取事实放进去就可以了。
 */
set< set<int> > MaxU::updateE(vector<Rule>& program) {
//    E.clear();
    set< set<int> > E;
    for(vector<Rule>::iterator it = program.begin(); it != program.end(); it++) {
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
    
    return E;
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
            p_sharp.push_back(*it);
        }    
    }
}
  

/*
 * 判断P*中是否存在满足要求的Loop
 * 如果存在，返回该Loop，
 * 如果不存在，则返回一个空的Loop
 */   
Loop MaxU::existLoop(FILE* out, const vector<Rule>& p_rules) {
    Loop result;
    
    DependenceGraph dpdg(p_rules);
    dpdg.findSCCs();
    vector<Loop> SCCs = dpdg.getSCCs();
    
    fprintf(out, "All SCCs in P* : \n");
    for(vector<Loop>::iterator sccit = SCCs.begin(); sccit != SCCs.end(); sccit++) {
        fprintf(out, "\t");
        for(set<int>::iterator lit = (sccit->loopNodes).begin(); lit != (sccit->loopNodes).end(); lit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*lit));
            if(lit != --((sccit->loopNodes).end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");
    }
    
    bool exist = false;         // 标记是否“存在满足要求的Loop”
    int i;
    // 因为一边循环中涉及对SCCs的插入操作，所以使用下标进行遍历
    for(i = 0; i < SCCs.size(); i++) {
        Loop S = SCCs.at(i);
        vector<Rule> es_rules = SCCs.at(i).ESRules;
        
        fprintf(out, "\nLoop : ");
        for(set<int>::iterator nit = S.loopNodes.begin(); nit != S.loopNodes.end(); nit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*nit));
            if(nit != --(S.loopNodes.end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\nES Rules : \n");
        for(vector<Rule>::iterator esit = S.ESRules.begin(); esit != S.ESRules.end(); esit++) {
            fprintf(out, "\t");
            esit->output(out);
        }
        fprintf(out, "\n"); fflush(out);
        
        // 如果isSubset为true，即当前外部支持的head(r)是L的子集，即当前Loop不是满足要求的环，继续判断下一个环。
        bool isSubset = false;
        for(vector<Rule>::iterator esit = es_rules.begin(); esit != es_rules.end(); esit++) {
            Rule rule = *esit;
            // 如果当前满足head(r)是L的子集，则当前这个Loop不符合要求，不用再判断后面的外部支持了，进而执行8.3.2。
            if(includes(S.loopNodes.begin(), S.loopNodes.end(), rule.heads.begin(), rule.heads.end())) {
                isSubset = true;
                break;
            }
        }
        
        // 如果isSubset为true，即当前Loop不满足要求，执行8.3.2，即把其所有下一级子环加入到SCCs中，并继续判断下一个环。
        if(isSubset) {
            fprintf(out, "Some ESRules maintain head(r) is subset of L\n");
            fprintf(out, "Goto Step 8.3.2, insert next sub SCC :\n");
            // 对原图分别减去每一个原子得到的图找SCC，并加入到SCCs中，然后继续循环SCCs。
            // 注意：这里找到SCC后，需要更新其外部支持。
            for(set<int>::iterator lit = S.loopNodes.begin(); lit != S.loopNodes.end(); lit++) {
                set<int> induceAtoms(S.loopNodes);
                induceAtoms.erase(induceAtoms.find(*lit));
                if(induceAtoms.size() > 0) {
                    DependenceGraph subgraph(p_rules);
                    map<int, set<int> > induceGraph = subgraph.induceSubgraph(induceAtoms);
                    subgraph.resetDpdGraph(induceGraph);

                    fprintf(out, "Subgraph : \n");
                    for(map<int, set<int> >::iterator igit = induceGraph.begin(); igit != induceGraph.end(); igit++) {
                        fprintf(out, "%s to : ", Vocabulary::instance().getAtomName(igit->first));
                        for(set<int>::iterator bigit = (igit->second).begin(); bigit != (igit->second).end(); bigit++) {
                            fprintf(out, "%s", Vocabulary::instance().getAtomName(*bigit));
                            if(bigit != --((igit->second).end()))
                                fprintf(out, ", ");
                        }
                        fprintf(out, "\n");
                    }
                    fprintf(out, "\n"); fflush(out);

                    if(subgraph.nodeNum() >= 1) {
                        subgraph.findSCCs();
                        vector<Loop> newSCCs = subgraph.getSCCs();
                        SCCs.insert(SCCs.end(), newSCCs.begin(), newSCCs.end());  

                        fprintf(out, "Insert new SCCs : \n");
                        for(vector<Loop>::iterator nsit = newSCCs.begin(); nsit != newSCCs.end(); nsit++) {
                            fprintf(out, "\t");
                            for(set<int>::iterator nit = (nsit->loopNodes).begin(); nit != (nsit->loopNodes).end(); nit++) { 
                                fprintf(out, "%s", Vocabulary::instance().getAtomName(*nit));
                                if(nit != --((nsit->loopNodes).end()))
                                    fprintf(out, ", ");
                            }
                            fprintf(out, "\n"); fflush(out);
                        }
                        fprintf(out, "\n"); fflush(out);
                    }
                }
                else {
                    fprintf(out, "No more next level subgraph\n");
                }
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
    // 如果是后者：exist为true，即“存在这样的一个loop，返回步骤6”。
    if(exist) {
        result = SCCs.at(i);    // 设置好result，然后返回，后面的返回步骤6则由调用函数的过程保证
    }
    // 如果是前者：exist为true，即“不存在这样的一个loop，执行步骤10”。
    // 其中步骤10的操作由调用函数中的过程保证。
//    else {     
//        U.insert(C.begin(), C.end());
//        // 不对result进行操作，后续直接返回一个空的Loop
//    }
    
    return result;
}


/*
 * 这里是把算法的步骤5～10整理为一个递归的过程，事实上它确实是个一个递归过程，
 * 因为步骤9返回步骤6,即循环E，而步骤10，返回步骤5,更新E，再次计算.
 */
void MaxU::Step5To10(FILE* out, vector<Rule>& rules) {
    set< set<int> > E = updateE(rules);
    
    // 输出E
    fprintf(out, "\nE : \n");
    for(set< set<int> >::iterator eit = E.begin(); eit != E.end(); eit++) {
        fprintf(out, "\t");
        for(set<int>::iterator sit = eit->begin(); sit != eit->end(); sit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*sit));
            if(sit != --(eit->end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n"); fflush(out);
    
    // 开始步骤6中的遍历当前的 E 
    fprintf(out, "Start to go through E : \n");
    for(set< set<int> >::iterator eit = E.begin(); eit != E.end(); eit++) {
        // 输出C
        fprintf(out, "C : ");
        for(set<int>::iterator cit = eit->begin(); cit != eit->end(); cit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*cit));
            if(cit != --(eit->end()))
                fprintf(out, ", ");
        }
        fprintf(out, "\n");     fflush(out);
        
        extractPSharp(*eit);                // 基于E中的元素C从P‘中得到P*
        fprintf(out, "\nP* is : \n");
        for(vector<Rule>::iterator psit = p_sharp.begin(); psit != p_sharp.end(); psit++) {
            fprintf(out, "\t");
            psit->output(out);
        }
        fprintf(out, "\n");     fflush(out);
        
        Loop that_loop = existLoop(out, p_sharp);       // 判断P*中是否存在符合要求的Loop
        
        
        // 如果that_loop为空，则“不存在这样的Loop”，existLoop()已经把C的元素插入U中，此时返回步骤5，即再次updateE()
        if(that_loop.loopNodes.empty()) {
            fprintf(out, "\nDo not exist that Loop, goto Step 10, update U to be : \n");
            // 执行步骤10
            U.insert(eit->begin(), eit->end());     // 把C中的元素都放进U中
            Utils::tr_p(p_quote, U);                // 用新的U完全化简P’
            
            fprintf(out, "\tcurrent U = { ");
            for(set<int>::iterator uit = U.begin(); uit != U.end(); uit++) {
                fprintf(out, "%s", Vocabulary::instance().getAtomName(*uit));
                if(uit != --(U.end()))
                    fprintf(out, ", ");
            }
            fprintf(out, " }\n");
            
            fprintf(out, "\nThe new P' is : \n");
            for(vector<Rule>::iterator pit = p_quote.begin(); pit != p_quote.end(); pit++) {
                pit->output(out);
            }
            fprintf(out, "\n");         fflush(out);
            
            // 结束循环，返回步骤5,即根据心的再次updateE，即跳出当前E的循环，并再次计算出的E。
            fprintf(out, "\nThen go to Step 5 : \n");
            Step5To10(out, p_quote);
        }
        // 如果that_loop不为空，则“存在这样的一个Loop，直接返回步骤6，即轮询下一个C，故直接继续循环即可。”
        else {
            fprintf(out, "Do exist that loop, goto Step 6.\nThe_Next C, "); // The next C
//          continue;
        }
    }
}


/*
 * 就算U的整个过程，调用上面的各个函数，
 * 一开始进行P’的完全化简，即执行步骤1～4，然后是实现步骤5～11这个过程。
 * @param out 仅是为了在测试过程中输出结果而用
 */
set<int> MaxU::calU(FILE* out) {
    simplifyProgram(out);          // 执行算法中的步骤1～4
    Step5To10(out, p_quote);    // 执行步骤5～10，直到所有E都被遍历结束，得到最后的U
    return U;
}


/*
 * 测试各个功能所用的辅助函数，具体测试内容见函数内部
 */
set<int> MaxU::MaxUTest(FILE* out) {
    fprintf(out, "\n=============================");
    fprintf(out, "\nStart the algorithm :\n");
    fprintf(out, "\nStep 1 to 4, simplify the program with U : \n");
    simplifyProgram(out);          // 执行算法中的步骤1～4
    fflush(out);
    
    fprintf(out, "\nStep 5 to 10 : \n");
    Step5To10(out, p_quote);    // 执行步骤5～10，直到所有E都被遍历结束，得到最后的U
       
    fprintf(out, "\n\nNo more C in E !");
    return U;
}
