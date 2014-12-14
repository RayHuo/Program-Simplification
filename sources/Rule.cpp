#include "Rule.h"
#include "structs.h"
#include "Vocabulary.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;
/*
 * 默认构造函数
 */
Rule::Rule() {
    head_length = 0;
    body_length = 0;
    heads.clear();
    bodys.clear();
    type = RULE;
}

/*
 * 自定义构造函数
 */
Rule::Rule(_rule* r) : head_length(r->head_length), body_length(r->body_length), type(r->type) {
    for(int i = 0; i < r->head_length; i++)
        heads.insert(r->head[i]);
    for(int i = 0; i < r->body_length; i++)
        bodys.insert(r->body[i]);
}

/*
 * 拷贝构造函数
 */
Rule::Rule(const Rule& r) : 
        heads(r.heads), head_length(r.head_length), 
        bodys(r.bodys), body_length(r.body_length), 
        type(r.type) {
    
}

/*
 * 析构函数
 */
Rule::~Rule(){
    head_length = 0;
    body_length = 0;
    heads.clear();
    bodys.clear();
    type = RULE;
}

/*
 * 等号运算符重载
 */
Rule& Rule::operator = (const Rule& r) {
    heads = r.heads;
    head_length = r.head_length;
    bodys = r.bodys;
    body_length = r.body_length;
    type = r.type;
    return *this;
}

/*
 * 等价运算符重载
 */
bool Rule::operator == (const Rule& r) {
    if(type != r.type || head_length != r.head_length || body_length != r.body_length)
        return false;
    else {
        if(!(heads.size() == r.heads.size() && 
                includes(heads.begin(), heads.end(), r.heads.begin(), r.heads.end())))
            return false;
        if(!(bodys.size() == r.bodys.size() && 
                includes(bodys.begin(), bodys.end(), r.bodys.begin(), r.bodys.end())))
            return false;
    }
    
    return true;
}

/*
 * 输出一个Rule，对应Rule可能的三种type进行输出。
 */
void Rule::output(FILE* out) const {
    // 打印事实
    if(type == FACT) {
        for(set<int>::const_iterator it = heads.begin(); it != heads.end(); it++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*it));
            if(it != --heads.end())
                fprintf(out, " | ");
        }
        fprintf(out, ".\n");
    }
    
    // 打印限制
    // 注意体部可能存在负数，对于负数的情况，需要加 not。
    if(type == CONSTRANT) {
        fprintf(out, ":- ");
        for(set<int>::const_iterator it = bodys.begin(); it != bodys.end(); it++) {
            int id = *it;
            if(id < 0) {
                fprintf(out, "not ");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(it != --bodys.end())
                fprintf(out, ", ");
        }
        fprintf(out, ".\n");
    }
    
    // 打印一般规则
    if(type == RULE) {
        // head
        for(set<int>::const_iterator it = heads.begin(); it != heads.end(); it++) {
            fprintf(out, "%s", Vocabulary::instance().getAtomName(*it));
            if(it != --heads.end())
                fprintf(out, " | ");
        }
        fprintf(out, " :- ");
        // body， 注意体部可能存在负数，对于负数的情况，需要加 not。
        for(set<int>::const_iterator it = bodys.begin(); it != bodys.end(); it++) {
            int id = *it;
            if(id < 0) {
                fprintf(out, "not ");
                id *= -1;
            }
            fprintf(out, "%s", Vocabulary::instance().getAtomName(id));
            if(it != --bodys.end())
                fprintf(out, ", ");
        }
        fprintf(out, ".\n");
    }
}

/*
 * 直接打印出Rule中的头部和体部的规则，及其类型
 */
void Rule::Situation(FILE* out) {
    fprintf(out, "Type %d :  ",type);
    for(set<int>::iterator it = heads.begin(); it != heads.end(); it++)
        fprintf(out, "%d, ", *it);
    fprintf(out, ":- ");
    for(set<int>::iterator it = bodys.begin(); it != bodys.end(); it++)
        fprintf(out, "%d, ", *it);
    fprintf(out, ".\n");
}