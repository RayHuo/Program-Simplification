#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structs.h"
#include <cstdio>
#include <vector>

using namespace std;
/*
 * 字符表类，保存各种信息
 */
class Vocabulary {
private:
    unsigned int atom_id;       // 在构造函数中默认初始化为1。
    vector<char*> atom_list;
private:
    Vocabulary();
    Vocabulary(const Vocabulary&);
    Vocabulary& operator =(const Vocabulary&);
    ~Vocabulary();
    
public:
    static Vocabulary& instance();
    void VocabularyDetails(FILE* out);
    int findAtom(const char *s);
    int insertAtom(const char *s);
    char* getAtomName(int id);
    int atomsNumber();
};

#endif
