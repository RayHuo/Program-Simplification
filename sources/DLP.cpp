#include "DLP.h"
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
DLP::DLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
}


/*
 * 自定义构造函数
 */
DLP::DLP(vector<Rule> p, set<int> l) : program(p), L(l) {
    Atoms_P.clear();
    for(vector<Rule>::const_iterator pit = program.begin(); pit != program.end(); pit++) {
        Atoms_P.insert(pit->heads.begin(), pit->heads.end());   // 因为head中必然是非负的
        for(set<int>::const_iterator bit = pit->bodys.begin(); bit != pit->bodys.end(); bit++) {
            Atoms_P.insert(abs(*bit));
        }
    }
}


/*
 * 析构函数
 */
DLP::~DLP() {
    program.clear();
    Atoms_P.clear();
    L.clear();
}


/**
 * 论文中algorithm 1的函数的weak版本。
 * @param X   开始的X为空集
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSWPL(set<int> X, FILE* out) {
    
}
    
/**
 * 论文中algorithm 1的函数的strong版本。
 * @param X
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSSPL(set<int> X, FILE* out) {
    
}


/**
 * 计算RSWPL的极小不动点
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSW(FILE* out) {
    
}


/**
 * 计算RSSPL的极小不动点
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::RSS(FILE* out) {
    
}


/**
 * 直接调用RWS得到结果
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::GWRS(FILE* out) {
    
}


/**
 * 直接调用RSS得到结果
 * @param out 输出中间结果，便于测试
 * @return 
 */
set<int> DLP::GSRS(FILE* out) {
    
}