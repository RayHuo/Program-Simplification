#include "Utils.h"
#include "Rule.h"
#include "Loop.h"
#include "structs.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

#define OUTPUT_FILE "IO/tmpHold.txt"
#define BUFF_SIZE 1024


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
 * 即tr_p(P, U)：基于U化简P
 * 化简的规则是，对于program中的rule：
 *      头部、负体中出现U中元素的删除该规则，
 *      正体中出现U中元素的删除该原子。
 * 所以这里的实际操作是：判断每条rule
 *      抽取rule的负体和正体;
 *      判断U与头部、负体和正体是否有交集，并执行相关的删除操作
 *      如果体部的原子个数为0,则需要把该rule的type修改为FACT
 */
void Utils::tr_p(vector<Rule>& program, set<int> U) {
    if(U.empty())
        return;
    
    for(vector<Rule>::iterator it = program.begin(); it != program.end(); ) {        
//        printf("U = ");
//        for(set<int>::const_iterator uit = U.begin(); uit != U.end(); uit++) {
//            if(*uit < 0)
//                printf("not ");
//            printf("%s, ", Vocabulary::instance().getAtomName(abs(*uit)));
//        }
//        printf("\n");   fflush(stdout);
//        it->output(stdout);     fflush(stdout);
        
        
        // 提取负体和正体
        set<int> pBody;
        set<int> nBody;         // {p | not p \in body(r)}
        set<int> negBody;       // {~p | not p \in body(r)}
        for(set<int>::iterator bit = (it->bodys).begin(); bit != (it->bodys).end(); bit++) {
            if(*bit >= 0)
                pBody.insert(*bit);
            else {
                nBody.insert(abs(*bit));
                negBody.insert(*bit);
            }
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
            set_difference(pBody.begin(), pBody.end(), U.begin(), U.end(), inserter(diff, diff.begin()));
            set<int> newBody;
            set_union(diff.begin(), diff.end(), negBody.begin(), negBody.end(), inserter(newBody, newBody.begin()));
            it->bodys = newBody;
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
    for (set<int>::const_iterator it = rule.bodys.begin();
            it != rule.bodys.end(); ++ it) {
        dnf.insert(- *it);
    }
    return dnf;
}


/*
 * R^-(X, P)，更为纯粹地基于一个集合来找外部支持
 * head(r) \cap X 不等于空集，且 body^+(r) \cap X = 空集
 */
vector<Rule> Utils::findESRules(const vector<Rule>& rules, set<int> X) {
    vector<Rule> esRules;
    for(vector<Rule>::const_iterator rit = rules.begin(); rit != rules.end(); rit++) {
        set<int> h_intersection;
        set_intersection(X.begin(), X.end(), (rit->heads).begin(), 
                (rit->heads).end(), inserter(h_intersection, h_intersection.begin()));
        if(!h_intersection.empty()) {
            set<int> b_intersection;    // 由于bodys里负原子是负数，正原子是整数，且X是atom集，不会有负数，所以可以直接算交集
            set_intersection(X.begin(), X.end(), (rit->bodys).begin(), 
                    (rit->bodys).end(), inserter(b_intersection, b_intersection.begin()));
            if(b_intersection.empty())
                esRules.push_back(*rit);
        }
    }
    
    return esRules;
}


/**
 * 调用clasp来计算NLP程序
 * @param fileName 调用clasp进行计算的程序
 * @return 输入程序的回答集
 */
void Utils::callClasp(string fileName, FILE* out) {
    // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    string cmd = "gringo " + fileName + " | clasp 100 > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    // 执行命令cmd，并可以通过其返回的FILE*对象，此处即pipe_file来读取执行的结果
    FILE *pipe_file = popen(cmd.c_str(), "r");
    // 通过下面的操作，把pipe_file中的结果写在一个实际的文件中。
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);
    
    // 获取计算时间
    string line, modelString, timeString, maohao, calModel, calTime;
    ifstream answer_in(OUTPUT_FILE);     
    while(getline(answer_in, line)) {
        if(line.substr(0, 6) == "Models") {
            stringstream ss(line);
            ss >> modelString >> maohao >> calModel;    // 读取输出中的 ：Models      : 2  
            fprintf(out, "%s\t%s\t", fileName.c_str(), calModel.c_str());
        }
        if(line.substr(0, 4) == "Time") {
            stringstream ss(line);
            ss >> timeString >> maohao >> calTime;    // 读取输出中的 ：Time        : 0.024s
            fprintf(out, "%s\n", calTime.c_str());
        }
    }
}


void Utils::callClaspD(string fileName, FILE* out) {
    // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    string cmd = "gringo " + fileName + " | claspD 100 > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    // 执行命令cmd，并可以通过其返回的FILE*对象，此处即pipe_file来读取执行的结果
    FILE *pipe_file = popen(cmd.c_str(), "r");
    // 通过下面的操作，把pipe_file中的结果写在一个实际的文件中。
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);
    
    // 获取计算时间
    string line, modelString, timeString, maohao, calModel, calTime;
    ifstream answer_in(OUTPUT_FILE);     
    while(getline(answer_in, line)) {
        if(line.substr(0, 6) == "Models") {
            stringstream ss(line);
            ss >> modelString >> maohao >> calModel;    // 读取输出中的 ：Models      : 2  
            fprintf(out, "%s\t%s\t", fileName.c_str(), calModel.c_str());
        }
        if(line.substr(0, 4) == "Time") {
            stringstream ss(line);
            ss >> timeString >> maohao >> calTime;    // 读取输出中的 ：Time        : 0.024s
            fprintf(out, "%s\n", calTime.c_str());
        }
    }
}


/**
 * 
 * @param fileName 进行计算的程序文件
 * @param out 输出统计数据，如解的个数，时间等数据。
 */
void Utils::callSmodels(string fileName, FILE* out) {
        // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    string cmd = "gringo " + fileName + " | smodels 100 > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    // 执行命令cmd，并可以通过其返回的FILE*对象，此处即pipe_file来读取执行的结果
    FILE *pipe_file = popen(cmd.c_str(), "r");
    // 通过下面的操作，把pipe_file中的结果写在一个实际的文件中。
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);
    
    // 获取计算时间
    string line, duration, calTime;
    ifstream answer_in(OUTPUT_FILE);     
    int answerNum = 0;
    while(getline(answer_in, line)) {
        if(line.substr(0, 7) == "Answer:") {
            answerNum++;
        }
        if(line.substr(0, 9) == "Duration:") {
            stringstream ss(line);
            ss >> duration >> calTime;    // 读取输出中的 ：Time        : 0.024s
            fprintf(out, "%s\t%d\t%s\n", fileName.c_str(), answerNum, calTime.c_str());
        }
    }
}


/**
 * 
 * @param fileName 进行计算的程序文件
 * @param out 输出统计数据，如解的个数，时间等数据。
 */
void Utils::callCmodels(string fileName, FILE* out) {
        // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    string cmd = "gringo " + fileName + " | cmodels 100 > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    // 执行命令cmd，并可以通过其返回的FILE*对象，此处即pipe_file来读取执行的结果
    long cmodelsStart = clock();
    FILE *pipe_file = popen(cmd.c_str(), "r");
    long cmodelsEnd = clock();
    double cmodelsTime = (double)(cmodelsEnd - cmodelsStart) / CLOCKS_PER_SEC;
    // 通过下面的操作，把pipe_file中的结果写在一个实际的文件中。
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);
    
    // 获取计算时间
    string line, duration, calTime;
    ifstream answer_in(OUTPUT_FILE);     
    int answerNum = 0;
    while(getline(answer_in, line)) {
        if(line.substr(0, 7) == "Answer:") {
            answerNum++;
        }
    }
    
    fprintf(out, "%s\t%d\t%.3f\n", fileName.c_str(), answerNum, cmodelsTime);
}


/**
 * 
 * @param fileName 进行计算的程序文件
 * @param out 输出统计数据，如解的个数，时间等数据。
 */
void Utils::callDLV(string fileName, FILE* out) {
        // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    string cmd = "dlv -n=100 " + fileName + " > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    // 执行命令cmd，并可以通过其返回的FILE*对象，此处即pipe_file来读取执行的结果
    long dlvStart = clock();
    FILE *pipe_file = popen(cmd.c_str(), "r");
    long dlvEnd = clock();
    double dlvTime = (double)(dlvEnd - dlvStart) / CLOCKS_PER_SEC;
    // 通过下面的操作，把pipe_file中的结果写在一个实际的文件中。
    FILE *output_file = fopen(OUTPUT_FILE, "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);
    
    // 获取计算时间
    string line, duration, calTime;
    ifstream answer_in(OUTPUT_FILE);     
    int answerNum = 0;
    while(getline(answer_in, line)) {
        if(line.substr(0, 1) == "{") {
            answerNum++;
        }
    }
    
    fprintf(out, "%s\t%d\t%.3f\n", fileName.c_str(), answerNum, dlvTime);
}