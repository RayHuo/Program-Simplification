#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structs.h"
#include <cstdio>
#include <vector>

using namespace std;
/*
 * 字符表类，保存parser过程中的atom
 * 由于其本身是一个全局唯一的对象类型，所以直接实现为单例形式
 */
class Vocabulary {
private:
    unsigned int atom_id;       // 在构造函数中默认初始化为1。
    vector<char*> atom_list;    // 存放所有atom的集合

// 单例模型，私有化构造函数和析构函数
private:
    Vocabulary();                               // 默认构造函数
    Vocabulary(const Vocabulary&);              // 拷贝构造函数
    Vocabulary& operator =(const Vocabulary&);  // 重载=运算符
    ~Vocabulary();                              // 析构函数
    
public:
    static Vocabulary& instance();              // 单例
    void VocabularyDetails(FILE* out);          // 输出Vocabulary中的atom信息
    int findAtom(const char *s);                // 通过atom的名称找到atom的id
    int insertAtom(const char *s);              // 插入atom
    char* getAtomName(int id);                  // 通过atom的id找到atom的名称
    int atomsNumber();                          // 返回当前Vocabulary中的atom数目
};

#endif
