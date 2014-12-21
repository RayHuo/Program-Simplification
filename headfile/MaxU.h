/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on December 8, 2014, 12:47 AM
 */

#ifndef MAXU_H
#define	MAXU_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Rule.h"
#include "Loop.h"
#include "Vocabulary.h"
#include "structs.h"
#include "DependenceGraph.h"

using namespace std;

/*
 * 实现笔记“20141130”中第三部分计算极大满足要求的U的算法
 */
class MaxU {
private:
    vector<Rule> p_quote;       // P’
    set<int> U;                 // 最后所求的U。
    set< set<int> > E;          // 保存析取事实的E, 由于步骤5～10是一个大循环，其中会修改E的值，所以不应把E作为一个属性
    vector<Rule> p_sharp;       // P*
    
public:
    MaxU();
    MaxU(vector<Rule> p);       // 使用形参来初始化p_quote。
    ~MaxU();
    
    // 计算极大U过程中用到的函数
    bool containFact();                 // 判断P'中是否还有事实。
    void initializeU();                 // 把P'中的事实放进U中来初始化U。 
    void simplifyProgram(FILE* out);             // 循环算法步骤2~4，直到把P‘完全化简
    void updateE(vector<Rule>& program);                     // 算法步骤5中，把当前程序的析取事实放进E中，注意：只需要把集合意义下最小的析取事实放进去就可以了。
    void extractPSharp(set<int> C);     // 从P'中根据C提取出P*
    Loop existLoop(FILE* out, const vector<Rule>& p_rules);                   // 判断P*中是否存在满足要求的Loop
    void Step5To10(FILE* out, vector<Rule>& rules);      // 这里是把算法的步骤5～10整理为一个递归的过程，事实上它确实是个一个递归过程
    
    set<int> calU(FILE* out);           // 就算U的整个过程，调用上面的各个函数，主要是实现步骤5～11这个过程。
    
    
    // 测试用辅助函数
    set<int> MaxUTest(FILE* out);           // 测试各个功能的函数
};

#endif	/* MAXU_H */