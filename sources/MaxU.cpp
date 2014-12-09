#include "MaxU.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

/*
 * 默认构造函数
 */
MaxU::MaxU() {
    p_quote.clear();
    U.clear();
    E.clear();
    p_sharp.clear();
}

/*
 * 使用指定的rule集合来初始化p_quote
 */
MaxU::MaxU(vector<Rule> p) : p_quote(p) {
    U.clear();
    E.clear();
    p_sharp.clear();
}

/*
 * 析构函数
 */
MaxU::~MaxU() {
    p_quote.clear();
    U.clear();
    E.clear();
    p_sharp.clear();
}
    

/*
 * 把P'中的事实放进U中来初始化U。
 * 仅把头部只含一个原子的事实放进U中。
 */
void MaxU::initializeU() {
    U.clear();
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); it++) {
        if(it->type == FACT && (it->heads).size() == 1) {
            U.insert(*((it->heads).begin()));
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
void MaxU::tr_p() {
    for(vector<Rule>::iterator it = p_quote.begin(); it != p_quote.end(); ) {
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
        if(!h_inter.empty()) {
            it = p_quote.erase(it);
        }
        
    }
}
    
/*
 * 循环算法步骤2~4，直到把P‘完全化简
 */
void MaxU::simplifyProgram() {
    
}
    
/*
 * 算法步骤5中，把当前程序的析取事实放进E中，注意：只需要把集合意义下最小的析取事实放进去就可以了。
 */
void MaxU::updateE() {
    
}
 
/*
 * 从P'中根据C提取出P*
 */
void MaxU::extractPSharp(set<int> C) {
    
}
    
   
/*
 * 就算U的整个过程，调用上面的各个函数，主要是实现步骤5～11这个过程。
 */
void MaxU::calU() {
    
}
