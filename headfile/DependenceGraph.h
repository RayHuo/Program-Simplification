/* 
 * File:   DependenceGraph.h
 * Author: ray
 *
 * Created on December 8, 2014, 11:34 PM
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
#include "Loop.h"


/*
 * 正依赖图类
 */
class DependenceGraph {
private:
    vector<Rule> input_rules;           // 正依赖图对应的程序
    vector<Loop> SCCs;                  // 该图中的所有SCC集合
    map<int, set<int> > dpdGraph;       // 原程序的有向正依赖图，key为head(r)，value为body^+(r)
    set<int> nodeSet;                   // 程序中的原子集合
    int maxNode;                        // 原子集合中id的最大值
    
    void tarjan(int u, vector<Loop>& loops);    // 寻找全连通分量的算法，时间复杂度仅为O(n)
    // tarjan算法中需要用到的全局变量。
    bool *visited;
    bool *involved;
    int *DFN;
    int *Low;
    int Index;
    stack<int> vs;
    
    
public:    
    DependenceGraph();                          // 默认构造函数
    DependenceGraph(vector<Rule> rules);        // 通过指定的一系列rules来构造对应的正依赖图。
    ~DependenceGraph();                         // 析构函数
    void findSCCs();                            // 找出图中的所有SCC，并存放在SCCs属性中
    vector<Loop> getSCCs();                     // 返回SCCs
    map<int, set<int> > getDpdGraph();          // 返回dpdGraph
    map<int, set<int> > induceSubgraph(set<int> atoms);    // 根据指定的atoms来得到原图的一个子图。
    void resetDpdGraph(map<int, set<int> > graph);         // 重置dpgGraph，这样可以让程序员选择，在获得子图后是否更新当前的graph
    int nodeNum();                                         // 直接返回nodeSet的size
    
    // 测试辅助函数
    void printDpdGraph(FILE* out);
    void printSCCs(FILE* out);
};

#endif	/* DEPENDENCEGRAPH_H */
