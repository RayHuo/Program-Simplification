 #include "Utils.h"
#include "Rule.h"
#include "Loop.h"
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
void Utils::findESRules(const vector<Rule>& rules, Loop& loop) {
    for(vector<Rule>::const_iterator it = rules.begin(); it != rules.end(); it++) {
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


/*
 * 即tr_p(P, U)，这里直接使用program与U作为参数，所以函数没有形参。
 * 化简的规则是，对于program中的rule：
 *      头部、负体中出现U中元素的删除该规则，
 *      正体中出现U中元素的删除该原子。
 * 所以这里的实际操作是：判断每条rule
 *      抽取rule的负体和正体;
 *      判断U与头部、负体和正体是否有交集，并执行相关的删除操作
 */
void Utils::tr_p(vector<Rule>& program, set<int> U) {
    for(vector<Rule>::iterator it = program.begin(); it != program.end(); ) {
        // 提取负体和正体
        set<int> pBody;
        set<int> nBody;
        for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
            if(*bit >= 0)
                pBody.insert(*bit);
            else
                nBody.insert(*bit);
        }
        
        set<int> h_inter;       // 头部与U的交集
        set<int> nb_inter;      // 负体与U的交集
        set<int> pb_inter;      // 正体与U的交集
        set_intersection(U.begin(), U.end(), (it->heads).begin(), (it->heads).end(), inserter(h_inter, h_inter.begin()));
        set_intersection(U.begin(), U.end(), nBody.begin(), nBody.end(), inserter(nb_inter, nb_inter.begin()));
        set_intersection(U.begin(), U.end(), pBody.begin(), pBody.end(), inserter(pb_inter, pb_inter.begin()));
        
        if(!h_inter.empty() || !nb_inter.empty()) {
            it = program.erase(it);
        }
        else if(!pb_inter.empty()) {
            set<int> diff;
            set_difference((it->bodys).begin(), (it->bodys).end(), U.begin(), U.end(), inserter(diff, diff.begin()));
            it->bodys = diff;
            it++;
        }
        else {
            it++;
        }
    }
}