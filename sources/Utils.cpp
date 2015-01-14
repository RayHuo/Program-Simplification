 #include "Utils.h"
#include "Rule.h"
#include "Loop.h"
#include "structs.h"
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
 *      如果体部的原子个数为0,则需要把该rule的type修改为FACT
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
        
        // 如果头部、负体中出现原子，删除该规则
        if(!h_inter.empty() || !nb_inter.empty()) {
            it = program.erase(it);
        }
        // 如果正体中出现原子，删除该原子
        else if(!pb_inter.empty()) {
            set<int> diff;
            set_difference((it->bodys).begin(), (it->bodys).end(), U.begin(), U.end(), inserter(diff, diff.begin()));
            it->bodys = diff;
            // 如果该rule的体部原子个数为0,则修改该rule的type为FACT
            if((it->bodys).size() == 0 || (it->bodys).empty())
                it->type = FACT;
            it++;
        }
        else {
            it++;
        }
    }
}



/*
 * 把一条rule转换成DNF的形式，注意应该考虑到DLP中的rule的例子，这个更具普遍性
 */
set<int> Utils::rule2DNF(const Rule& rule) {
    set<int> dnf;
    // 头部直接加入
    for (set<int>::const_iterator it = rule.heads.begin();
            it != rule.heads.end(); ++ it) {
        dnf.insert(*it);
    }
    // 体部取非再加入
    for (set<int>::const_iterator it = rule.bodys.end();
            it != rule.bodys.end(); ++ it) {
        dnf.insert(- *it);
    }
    return dnf;
}
