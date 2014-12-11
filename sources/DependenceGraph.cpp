/* 
 * File:   DependenceGraph.cpp
 * Author: ray
 * 
 * Created on December 8, 2014, 11:34 PM
 */

#include "DependenceGraph.h"
#include "Utils.h"
#include "Loop.h"
#include "Rule.h"
#include "structs.h"
#include "Vocabulary.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <vector>
#include <map>
#include <string.h>
#include <set>
#include <algorithm>
#include <functional>
#include <sstream>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;


/*
 * 默认构造函数
 */
DependenceGraph::DependenceGraph() {  
    maxNode = 0;
    Index = 0;
    input_rules.clear();
    SCCs.clear();
    dpdGraph.clear();
    nodeSet.clear();
    
    visited = new bool[2];;
    memset(visited, false,sizeof(bool) * (2));
    DFN = new int[2];
    memset(DFN, 0, sizeof(int) * (2));
    Low = new int[2];
    memset(Low, 0, sizeof(int) * (2));
    involved = new bool[2];
    memset(involved, true, sizeof(bool) * (2));
}

/*
 * 通过参数rules来构建其对应的正依赖图。
 */
DependenceGraph::DependenceGraph(vector<Rule> rules) : input_rules(rules) {
    nodeSet.clear(); 
    for(vector<Rule>::iterator it = rules.begin(); it != rules.end(); it++) {
        for(set<int>::iterator hit = (it->heads).begin(); hit != (it->heads).end(); hit++) {
            dpdGraph.insert(make_pair(*hit, set<int>()));
            nodeSet.insert(*hit);
        }
        for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
            if(*bit > 0) {
                dpdGraph.insert(make_pair(*bit, set<int>()));
                nodeSet.insert(*bit); 
            }
        }
        if(it->type == RULE) {
            // heads里边的元素必然都是正的。
            for(set<int>::iterator hit = (it->heads).begin(); hit != (it->heads).end(); hit++) {
                for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
                    if(*bit > 0) {
                        dpdGraph[*hit].insert(*bit);
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
    SCCs.clear();
    input_rules.clear();
    nodeSet.clear();
    
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
        Loop l;
        if(vs.top() != u) {
            while(vs.top() != u) {
                l.loopNodes.insert(vs.top());
                vs.pop();
            }
        }
        l.loopNodes.insert(u);
        Utils::findESRules(input_rules, l);
        vs.pop();
        loops.push_back(l);
    }
}


/*
 * 返回SCCs.
 */
vector<Loop> DependenceGraph::getSCCs() {
    return SCCs;
}


/*
 * 返回dpdGraph
 */
map<int, set<int> > DependenceGraph::getDpdGraph() {
    return dpdGraph;
}


/*
 * 根据指定的atoms来得到原图的一个子图，实际为原图减去atoms中的点。
 * 另外：由于在使用findSCCs()时，需要重置visited，然后maxNode必然比当前的要大，所以不需要担心。
 */
map<int, set<int> > DependenceGraph::induceSubgraph(set<int> atoms) {
    map<int, set<int> > subgraph(dpdGraph);
    
    // 先把作为出发点的都删掉
    for(set<int>::iterator it = atoms.begin(); it != atoms.end(); it++)
        subgraph.erase(*it);
    
    // 接着把作为到达点的删掉，即每个second都跟atoms进行一个差集即可
    for(map<int, set<int> >::iterator it = subgraph.begin(); it != subgraph.end(); it++) {
        set<int> diff;
        set_difference((it->second).begin(), (it->second).end(), atoms.begin(), atoms.end(), inserter(diff, diff.begin()));
        it->second = diff;
    }

    return subgraph;
}

/*
 * 更新dpdGraph
 */
void DependenceGraph::resetDpdGraph(map<int, set<int> > graph) {
    dpdGraph = graph;
    // 要把nodeSet也一并更新
    nodeSet.clear();
    for(map<int, set<int> >::iterator kit = dpdGraph.begin(); kit != dpdGraph.end(); kit++) {
        nodeSet.insert(kit->first);
        for(set<int>::iterator vit = (kit->second).begin(); vit != (kit->second).end(); kit++) {
            nodeSet.insert(*vit);
        }
    }
    maxNode = *(--(nodeSet.end()));
}

/*
 * 返回本正依赖图中的原子个数
 */
int DependenceGraph::nodeNum() {
    return nodeSet.size();
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
        fprintf(out, "\n\tES rules : \n");
        for(vector<Rule>::iterator esit = (it->ESRules).begin(); esit != (it->ESRules).end(); esit++) {
            fprintf(out, "\t"); esit->output(out);
        }
    }
    fprintf(out, "\n");
}