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

using namespace std;

/**
 * 工具类
 */
class Utils {
public:
    static void findESRules(const vector<Rule>& rules, Loop& loop);    // 在指定的rule集中查找指定loop的外部支持。
    
};



#endif	/* UTIL_H */

