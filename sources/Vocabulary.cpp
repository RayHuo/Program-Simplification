#include "Vocabulary.h"
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <assert.h>

/*
 * 单例函数，Vocabulary负责保存当前输入文件中的所有的atom，
 * 由于全局不变，所以以单例形式存在于程序中。
 */
Vocabulary& Vocabulary::instance() {
    static Vocabulary vocabulary;
    return vocabulary;
}

/*
 * 默认构造函数
 * @param 1 初始化atom_id为1，即所有的atom的序号从1开始计算
 * @return no return
 */
Vocabulary::Vocabulary() : atom_id(1) {
    atom_list.clear();
}

/*
 * 析构函数
 * atom_list直接被清空，注意atom_list的元素类型是char*，所以清空时需要回收内存。
 * atom_id置为-1。
 */
Vocabulary::~Vocabulary() {
    for(vector<char*>::iterator it = atom_list.begin(); it != atom_list.end(); it++)
        free(*it);
    atom_list.clear();
    atom_id = -1;
}

/*
 * 输出Vocabulary中的所有atom及其id
 * @param out   输出的文件流对象
 * @return no return
 */
void Vocabulary::VocabularyDetails(FILE* out) {
    fprintf(out, "Vocabulary List : \n");
    int i = 1;  // 默认的atom序号是从1开始的。
    for(vector<char*>::iterator it = atom_list.begin(); it != atom_list.end(); it++)
        fprintf(out, "%s with id %i\n", (*it), i++);
    fprintf(out, "\n");
}
 
/*
 * 根据给定的atom名字(字符串)来查找该atom的id。
 * @param s atom名字
 * @return 返回该atom的id，如果找不到该atom，返回-1。
 */
int Vocabulary::findAtom(const char *s) {
    for(int i = 0; i < atom_list.size(); i++)
        if(strcmp(s, atom_list.at(i)) == 0)
            return i + 1;
    return -1;
}

/*
 * 通过atom的名字(字符串)把atom插入到Vocabulary的atom_list中;
 * 先判断该atom是否已经存在于atom_list中，如果是，直接返回其id，
 * 否则插入到atom_list中，并返回其id。
 * @param s 要插入的atom的名字
 * @return 返回该atom对应的id
 */
int Vocabulary::insertAtom(const char *s) {
   int id = findAtom(s);
   if(id < 0) {
       atom_list.push_back(strdup(s));   // 这里注意要深拷贝，所以析构函数中需要用free来回收内存.
       return atom_id++;
   }
   return id;
}

/*
 * 根据atom的id来查找其名字
 * @param id 需要查找的atom的id
 * @return 返回该atom的名字（字符串）
 */
char* Vocabulary::getAtomName(int id) {
    return atom_list.at(id - 1);
}

/*
 * 返回atom_id，即atom_list的size
 * @param no param
 * @return 返回atom_id
 */
int Vocabulary::atomsNumber() {
    return atom_id;
}
