#include "Loop.h"
#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

Loop::Loop() {
    loopNodes.clear();
    ESRules.clear();
}
    
Loop::Loop(set<int> s) {
    loopNodes = s;
    ESRules.clear();
}
    
Loop::Loop(const Loop& l) {
    loopNodes = l.loopNodes;
    ESRules = l.ESRules;
}
    
    
Loop::~Loop() {
    loopNodes.clear();
    ESRules.clear();
}
    
    
bool Loop::operator==(const Loop& l) {
    if(this->loopNodes.size() != l.loopNodes.size()) return false;

    for(set<int>::iterator it = this->loopNodes.begin(), lit = l.loopNodes.begin(); 
            it != this->loopNodes.end(); lit++, it++) {
        if(*it != *lit) return false;
    }

    return true;
}
    
    
int Loop::calHash() {
    int hash = 0;
    for(set<int>::iterator it = loopNodes.begin(); it != loopNodes.end(); it++) {
       hash += (*it) * (*it);
    } 

    return hash;
}
    
    
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
        it->output(out);
    }
    fprintf(out, "\n");
}
