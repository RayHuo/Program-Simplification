#ifndef STRUCTS_H
#define	STRUCTS_H

#define MAX_ATOM_LENGTH 2048

#include <map>
#include <set>
#include <vector>


// 规则类型，规则固定为头部析取，体部合取。
enum RULE_TYPE {
    FACT = 0,   // 事实，a :-. 或 a. ( a | b | c :-. 或 a | b | c.)
    CONSTRANT,  // 限制，:- a. 或 :- a, b, c.
    RULE        // 一般规则。a | b | c :- d, e, f.
};


/*
 * 规则结构(rule)
 * rule由头部和体部组成，
 * 由于头部只能析取，体部只能合取，所以只记录头部和体部的文字在Vocabulary中的序号即可，
 * 其中“如果a = 1, 则not a = -1”
 */
typedef struct __rule {
    int head[MAX_ATOM_LENGTH];  // 头部文字的序号集合
    int body[MAX_ATOM_LENGTH];  // 体部文字的序号集合
    int head_length;            // 头部中的文字数量
    int body_length;            // 体部中的文字数量
    RULE_TYPE type;             // rule的类型，上面提及的3种之一。
}_rule;


/*
 * 文字集结构。
 * 如上所述，头部和体部均可以以一个文字集合来表示，
 * 所以在parser中，head和body都为_literals类型，
 * 有具体内容和数量两个属性。
 */
typedef struct __literals {
    int atoms[MAX_ATOM_LENGTH]; // 文字集中的具体文字集合。
    int atoms_length;           // 文字集中文字的个数。
}_literals;

#endif

