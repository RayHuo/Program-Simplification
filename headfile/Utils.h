/* 
 * File:   Util.h
 * Author: yzf
 *
 * Created on July 27, 2013, 1:33 AM
 */

#ifndef UTIL_H
#define	UTIL_H

#include <cstdio>
#include "structs.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include "Vocabulary.h"
#include "Rule.h"
#include "DependenceGraph.h"
#include "Loop.h"

using namespace std;

/**
 * 工具类
 */
class Utils {
public:
    static void findESRules(const vector<Rule>& rules, Loop& loop);    // 在指定的rule集中查找指定loop的外部支持。
    static void tr_p(vector<Rule>& program, set<int> U);               // 即tr_p(P, U)：头部、负体中出现删除规则，正体中出现删除此原子

    // For Greatest Reliable Set
    static set<int> rule2DNF(const Rule& rule);         // DNF就是一个文字集，负文字用对应的负数表示即可
    

};



#endif	/* UTIL_H */

