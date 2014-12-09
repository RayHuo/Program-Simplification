/* 
 * File:   DependenceGraph.cpp
 * Author: ray
 * 
 * Created on December 8, 2014, 11:34 PM
 */

#include "DependenceGraph.h"
#include <iostream>
#include <cstdio>
#include "Utils.h"
#include <cstring>
#include <assert.h>
#include <vector>
#include "Rule.h"
#include <map>
#include "structs.h"
#include "Vocabulary.h"
#include <string.h>
#include <set>
#include <algorithm>
#include <functional>
#include <sstream>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

extern vector<Rule> G_Rules;

DependenceGraph::DependenceGraph() {  
    for(vector<Rule>::iterator it = G_Rules.begin(); it != G_Rules.end(); it++) {
        // heads里边的元素必然都是正的。
        for(set<int>::iterator hit = (it->heads).begin(); hit != (it->heads).end(); hit++) {
            for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
                if(*bit > 0)
                    dpdGraph[*hit].insert(*bit);
            }
        }
    }
    
    // 这个主要用于给出tarjan算法里的visited的size.
    int maxNode = Vocabulary::instance().atomsNumber();
    visited = new bool[maxNode + 1];
    memset(visited, false,sizeof(bool) * (maxNode + 1));
    DFN = new int[maxNode + 1];
    memset(DFN, 0, sizeof(int) * (maxNode + 1));
    Low = new int[maxNode + 1];
    memset(Low, 0, sizeof(int) * (maxNode + 1));
    involved = new bool[maxNode + 1];
    memset(involved, true, sizeof(bool) * (maxNode + 1));
    Index = 0;
    SCCs.clear();
}


DependenceGraph::~DependenceGraph() {
    dpdGraph.clear();
    delete[] visited;
    delete[] DFN;
    delete[] Low;
    delete[] involved;
    while(!vs.empty())
        vs.pop();
    Index = -1;
}


void DependenceGraph::findSCCs() {
    memset(visited, false, sizeof(bool) * (Vocabulary::instance().atomsNumber() + 1));
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        if(!visited[it->first] && involved[it->first]) {
            Index = 0;
            tarjan(it->first, SCCs);
        }
    }
}


void DependenceGraph::tarjan(int u, vector<Loop>& loops) {
    DFN[u] = Low[u] = ++Index;
    vs.push(u);
    visited[u] = true;
    
    for(set<int>::iterator it = dpdGraph[u].begin(); it != dpdGraph[u].end(); it++) {
        if(!involved[*it])
            continue;
        if(!visited[*it]) {
            tarjan(*it, loops);
            if(Low[u] > Low[*it])
                Low[u] = Low[*it];
        }
        else {
            if(Low[u] > DFN[*it])
                Low[u] = DFN[*it];
        }
    }
    
    if(Low[u] == DFN[u]) {
        if(vs.top() != u) {
            Loop l;
            while(vs.top() != u) {
                l.loopNodes.insert(vs.top());
                vs.pop();
            }
            l.loopNodes.insert(u);
            findESRules(l);
            vs.pop();
            loops.push_back(l);
        }
        else {
            vs.pop();
        }
    }
}

void DependenceGraph::findESRules(Loop& loop) {
    int i = 1;  // 外部支持存放的是rule的序号，从1开始。
    for(vector<Rule>::iterator it = G_Rules.begin(); it != G_Rules.end(); it++, i++) {
        set<int> h_intersection;
        set_intersection(loop.loopNodes.begin(), loop.loopNodes.end(), (it->heads).begin(), 
                (it->heads).end(), inserter(h_intersection, h_intersection.begin()));
        if(!h_intersection.empty()) {
            set<int> b_intersection;
            set_intersection(loop.loopNodes.begin(), loop.loopNodes.end(), (it->bodys).begin(),
                    (it->bodys).end(), inserter(b_intersection, b_intersection.begin()));
            if(b_intersection.empty())
                loop.ESRules.insert(i);
        }
    }
}