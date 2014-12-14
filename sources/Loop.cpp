#include "Loop.h"
#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

/*
 * 默认构造函数，直接清空loopNodes和ESRules
 */
Loop::Loop() {
    loopNodes.clear();
    ESRules.clear();
}
   
/*
 * 带参数的构造函数，使用一个set<int> s来初始化loopNodes，并清空ESRules.
 * 由于ESRules是根据特定程序来确定，的所以不需要或不应该在构造函数给出，应该使用
 * Utils::findESRules(vector<Rule> program, Loop& loop)来直接更新一个loop
 * 的外部支持
 */
Loop::Loop(set<int> s) {
    loopNodes = s;
    ESRules.clear();
}
    
/*
 * 拷贝构造函数
 */
Loop::Loop(const Loop& l) {
    loopNodes = l.loopNodes;
    ESRules = l.ESRules;
}
    
/*
 * 析构函数
 */    
Loop::~Loop() {
    loopNodes.clear();
    ESRules.clear();
}
    
/*
 * 重载==运算符，如果两个环的loopNodes完全相等，则两个环相等
 */    
bool Loop::operator==(const Loop& l) {
    if(this->loopNodes.size() != l.loopNodes.size()) return false;

    for(set<int>::iterator it = this->loopNodes.begin(), lit = l.loopNodes.begin(); 
            it != this->loopNodes.end(); lit++, it++) {
        if(*it != *lit) return false;
    }

    return true;
}
    
/*
 * 以环中所有节点在Vocabulary中的序号的平方和来作为hash值，
 * 用于快速判断两个环是否相等。如果判断效果不佳，可以考虑在比
 * 较完平方和之后再比较立方和，，两者都相等才判定为相等。
 */    
int Loop::calHash() {
    int hash = 0;
    for(set<int>::iterator it = loopNodes.begin(); it != loopNodes.end(); it++) {
       hash += (*it) * (*it);
    } 

    return hash;
}
    
/*
 * 输出环，先输出环的节点，再输出环的外部支持
 */    
void Loop::printLoop(FILE* out) {
    fprintf(out, "Loop: ");
    for(set<int>::iterator it = this->loopNodes.begin(); it != this->loopNodes.end(); it++) {
        fprintf(out, "%s ", Vocabulary::instance().getAtomName(*it));
    }
    //fprintf(out, "\n");
    fprintf(out, "ES: ");
    for(vector<Rule>::iterator it = this->ESRules.begin(); it != this->ESRules.end(); it++) {
       //G_NLP[*it].output(stdout);
//            fprintf(out, "%d ", *it);
        fprintf(out, "\t");
        it->output(out);
    }
    fprintf(out, "\n");
}
