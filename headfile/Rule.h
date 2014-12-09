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
 */
class Rule {
public:
    int head_length;
    int body_length;
    set<int> bodys;
    set<int> heads;
    RULE_TYPE type;
public:
    Rule();
    Rule(_rule* r);
    Rule(const Rule& r);
    ~Rule();
    Rule& operator = (const Rule& r);
    bool operator == (const Rule& r);
    void output(FILE* out) const;
    void Situation(FILE* out);
};

#endif	/* RULE_H */

