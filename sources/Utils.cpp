 #include "Utils.h"
#include "Rule.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

/*
 * 在指定的rule集中给指定的Loop找出其所有外部支持。
 * 注意，这里的形参是call by reference，即会直接修改参数本身中的ESRules属性.
 */
Utils::findESRules(const vector<Rule>& rules, Loop& loop) {
    for(vector<Rule>::iterator it = rules.begin(); it != rules.end(); it++) {
        set<int> h_intersection;
        set_intersection(loop.loopNodes.begin(), loop.loopNodes.end(), (it->heads).begin(), 
                (it->heads).end(), inserter(h_intersection, h_intersection.begin()));
        if(!h_intersection.empty()) {
            set<int> b_intersection;
            set_intersection(loop.loopNodes.begin(), loop.loopNodes.end(), (it->bodys).begin(),
                    (it->bodys).end(), inserter(b_intersection, b_intersection.begin()));
            if(b_intersection.empty())
                loop.ESRules.push_back(*it);
        }
    }
}

