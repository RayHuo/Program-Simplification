/* 
 * File:   Rule.h
 * Author: ray
 *
 * Created on December 8, 2014, 9:59 AM
 */

#ifndef LOOP_H
#define	LOOP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include "Rule.h"
#include "Vocabulary.h"

using namespace std;


class Loop {
public:
    set<int> loopNodes;
    vector<Rule> ESRules;
    
    Loop();
    Loop(set<int> s);
    Loop(const Loop& l);   
    ~Loop();    
    bool operator==(const Loop& l);   
    int calHash();    
    void printLoop(FILE* out);
};

#endif	/* LOOP_H */