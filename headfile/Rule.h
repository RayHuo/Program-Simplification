/* 
 * File:   Rule.h
 * Author: ray
 *
 * Created on December 8, 2014, 9:59 AM
 */

#ifndef RULE_H
#define	RULE_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "structs.h"

using namespace std;
/*
 * 规则类
 * 一个规则由头部和体部组成，其中由于头部只能析取，体部只能合取，
 * 所以直接使用头部节点集和体部节点集即可表示一个rule
 */
class Rule {
public:
    int head_length;    // 头部文字的数量
    int body_length;    // 体部文字的数量
    set<int> bodys;     // 体部文字集合
    set<int> heads;     // 头部文字集合
    RULE_TYPE type;     // rule的类型：FACT、CONSTRAINT、RULE
public:
    Rule();                             // 默认构造函数
    Rule(_rule* r);                     // 带参数的构造函数，以一个struct _rule来构造一个Rule对象。     
    Rule(const Rule& r);                // 拷贝构造函数
    ~Rule();                            // 析构函数
    Rule& operator = (const Rule& r);   // 重载=运算符
    bool operator == (const Rule& r);   // 重载==运算符
    void output(FILE* out) const;       // 打印出Rule的实际形式
    void Situation(FILE* out);          // 打印出Rule中的文字序号。
};

#endif	/* RULE_H */

