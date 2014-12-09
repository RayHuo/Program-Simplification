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


/*
 * 默认构造函数
 */
DependenceGraph::DependenceGraph() {  
    maxNode = 0;
    Index = 0;
    SCCs.clear();
    dpdGraph.clear();
}

/*
 * 通过参数rules来构建其对应的正依赖图。
 */
DependenceGraph::DependenceGraph(vector<Rule> rules) {
    set<int> nodeSet; 
    for(vector<Rule>::iterator it = rules.begin(); it != rules.end(); it++) {
        if(it->type == RULE) {
            // heads里边的元素必然都是正的。
            for(set<int>::iterator hit = (it->heads).begin(); hit != (it->heads).end(); hit++) {
                nodeSet.insert(*hit);
                for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
                    if(*bit > 0) {
                        dpdGraph[*hit].insert(*bit);
                        nodeSet.insert(*bit);
                    }
                }
            }
        }
    }
    
    // 这个主要用于给出tarjan算法里的visited的size.
//    int maxNode = Vocabulary::instance().atomsNumber();
    maxNode = *(--nodeSet.end());
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

/*
 * 析构函数
 */
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

/*
 * 找出程序正依赖图中的所有SCC。
 */
void DependenceGraph::findSCCs() {
    memset(visited, false, sizeof(bool) * (maxNode + 1));
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        if(!visited[it->first] && involved[it->first]) {
            Index = 0;
            tarjan(it->first, SCCs);
        }
    }
}

/*
 * tarjan算法，以O(n)的时间复杂度，把输入的有向图的所有SCC都找出来，并保存在SCCs中。
 * 在找到一个SCC的同时，还会将其外部支持找到，实际操作是将各个外部支持rule在输入程序中
 * 的序号存放在loop.ESRules中。
 */
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

/*
 * 给指定的Loop找出其所有外部支持，并保存该外部支持在输入程序中的序号。
 * 注意，这里的形参是call by reference，即会直接修改参数本身中的ESRules属性.
 */
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


/*
 * 打印出正依赖图
 */
void DependenceGraph::printDpdGraph(FILE* out) {
    fprintf(out, "\ndpdGraph\n");
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        if(it->first < 0) {
            printf("\nPrint DpdGraph Error!\n");
            assert(0);
        } 
        fprintf(out, "%s to ", Vocabulary::instance().getAtomName(it->first));
        for(set<int>::iterator bit = (it->second).begin(); bit != (it->second).end(); bit++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*bit));
            if(bit != --(it->second).end())
                fprintf(out, ", ");
        }
        fprintf(out, ".\n");
    }
}


/*
 * 把SCCs打印出来
 */
void DependenceGraph::printSCCs(FILE* out) {
    fprintf(out, "\nAll SCCs : \n");
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
        fprintf(out, "SCC : ");
        for(set<int>::iterator nit = (it->loopNodes).begin(); nit != (it->loopNodes).end(); nit++) {
            if(*nit < 0) {
                printf("\nPrint SCC Error\n");
                assert(0);
            }
            fprintf(out, "%s ", Vocabulary::instance().getAtomName(*nit));
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}