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

extern vector<Rule> G_Rules;

struct Loop {
    set<int> loopNodes;
    set<int> ESRules;   // 存放外部支持rule在G_Rules中的下标号，从0开始
    
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
    vector<Loop> SCCs;
    map<int, set<int> > dpdGraph;       // 原程序的有向正依赖图，key为head(r)，value为body^+(r)
    
    void tarjan(int u, vector<Loop>& loops);    // 寻找全连通分量的算法，时间复杂度仅为O(n)
    // tarjan算法中需要用到的全局变量。
    bool *visited;
    bool *involved;
    int *DFN;
    int *Low;
    int Index;
    stack<int> vs;
    
public:
    DependenceGraph();  // 直接在默认构造函数中把整个正依赖图构造出来。
    ~DependenceGraph();
    void findSCCs();
    void findESRules(Loop& loop);
};

#endif	/* DEPENDENCEGRAPH_H */
