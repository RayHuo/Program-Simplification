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
    set< set<int> > E;          // 保存析取事实的E
    vector<Rule> p_sharp;       // P*
    
public:
    MaxU();
    MaxU(vector<Rule> p);       // 使用形参来初始化p_quote。
    ~MaxU();
    
    // 计算极大U过程中用到的函数
    void initializeU();                 // 把P'中的事实放进U中来初始化U。 
    void tr_p();                        // 即tr_p(P, U)，这里直接使用program与U作为参数，所以函数没有形参。
    void simplifyProgram();             // 循环算法步骤2~4，直到把P‘完全化简
    void updateE();                     // 算法步骤5中，把当前程序的析取事实放进E中，注意：只需要把集合意义下最小的析取事实放进去就可以了。
    void extractPSharp(set<int> C);     // 从P'中根据C提取出P*
    
    void calU();                        // 就算U的整个过程，调用上面的各个函数，主要是实现步骤5～11这个过程。
    
};

#endif	/* MAXU_H */