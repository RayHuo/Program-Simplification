/* 
 * File:   Rule.h
 * Author: ray
 *
 * Created on December 8, 2014, 9:59 AM
 */

#ifndef LOOP_H
#define	LOOP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include "Rule.h"
#include "Vocabulary.h"

using namespace std;

/*
 * 环类，用于表示程序正依赖图中的环，包含环节点及该环的外部支持两个属性。
 */
class Loop {
public:
    set<int> loopNodes;                 // 保存环节点的属性，环里的原子在Vocabulary中的序号集合
    vector<Rule> ESRules;               // 保存环外部支持规则的属性，直接保存该环的所有外部支持
    
    Loop();                             // 默认构造函数
    Loop(set<int> s);                   // 使用一个节点集合作为参数的构造参数
    Loop(const Loop& l);                // 拷贝构造函数
    ~Loop();                            // 析构函数
    bool operator==(const Loop& l);     // 重载判断相等运算符
    int calHash();                      // 计算一个环的hash值，用于快速判重。
    void printLoop(FILE* out);          // 打印一个环。
};

#endif	/* LOOP_H */