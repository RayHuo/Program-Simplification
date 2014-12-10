/* 
 * File:   DependenceGraph.h
 * Author: sharpen
 *
 * Created on September 3, 2013, 11:34 PM
 */

#ifndef DEPENDENCEGRAPH_H
#define	DEPENDENCEGRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include "Rule.h"
#include "Utils.h"


struct Loop {
    set<int> loopNodes;
    vector<Rule> ESRules;
    
    Loop() {
        loopNodes.clear();
        ESRules.clear();
    }
    Loop(set<int> s) {
        loopNodes = s;
        ESRules.clear();
    }
    Loop(const Loop& l) {
        loopNodes = l.loopNodes;
        ESRules = l.ESRules;
    }
    
    ~Loop() {
        loopNodes.clear();
        ESRules.clear();
    }
    
    bool operator==(const Loop& l) {
        if(this->loopNodes.size() != l.loopNodes.size()) return false;
        
        for(set<int>::iterator it = this->loopNodes.begin(), lit = l.loopNodes.begin(); 
                it != this->loopNodes.end(); lit++, it++) {
            if(*it != *lit) return false;
        }
        
        return true;
    }
    
    int calHash() {
        int hash = 0;
        for(set<int>::iterator it = loopNodes.begin(); it != loopNodes.end(); it++) {
           hash += (*it) * (*it);
        } 
        
        return hash;
    }
    
    void printLoop(FILE* out) {
        fprintf(out, "Loop: ");
        for(set<int>::iterator it = this->loopNodes.begin(); it != this->loopNodes.end(); it++) {
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(*it));
        }
        //fprintf(out, "\n");
        fprintf(out, "ES: ");
        for(set<int>::iterator it = this->ESRules.begin(); it != this->ESRules.end(); it++) {
           //G_NLP[*it].output(stdout);
            fprintf(out, "%d ", *it);
        }
        fprintf(out, "\n");
    }
};

class DependenceGraph {
private:
    vector<Rule> input_rules;
    vector<Loop> SCCs;
    map<int, set<int> > dpdGraph;       // 原程序的有向正依赖图，key为head(r)，value为body^+(r)
    int maxNode;
    
    void tarjan(int u, vector<Loop>& loops);    // 寻找全连通分量的算法，时间复杂度仅为O(n)
    // tarjan算法中需要用到的全局变量。
    bool *visited;
    bool *involved;
    int *DFN;
    int *Low;
    int Index;
    stack<int> vs;
    
    
public:    
    DependenceGraph();
    DependenceGraph(vector<Rule> rules);        // 通过指定的一系列rules来构造对应的正依赖图。
    ~DependenceGraph();
    void findSCCs();
//    void findESRules(const vector<Rule>& rules, Loop& loop);
    vector<Loop> getSCCs();
    map<int, set<int> > induceSubgraph(set<int> atoms);    // 根据指定的atoms来得到原图的一个子图。
    void resetDpdGraph(map<int, set<int> > graph);         // 重置dpgGraph，这样可以让程序员选择，在获得子图后是否更新当前的graph
    
    // 测试辅助函数
    void printDpdGraph(FILE* out);
    void printSCCs(FILE* out);
};

#endif	/* DEPENDENCEGRAPH_H */
