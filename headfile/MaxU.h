/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on December 8, 2014, 12:47 AM
 */

#ifndef MAXU_H
#define	MAXU_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Rule.h"
#include "Vocabulary.h"
#include "structs.h"
#include "DependenceGraph.h"

using namespace std;

class MaxU {
private:
    vector<Rule> program;
    set<int> U;
    set< set<int> > E;
public:
    MaxU();
    ~MaxU();
    
    // 计算极大U过程中用到的函数
    void initializeU();
    void tr_p();
    
};

#endif	/* MAXU_H */