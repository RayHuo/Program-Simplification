#ifndef STRUCTS_H
#define	STRUCTS_H

#define MAX_ATOM_LENGTH 2048

#include <map>
#include <set>
#include <vector>


enum RULE_TYPE {
    FACT = 0,
    CONSTRANT,
    RULE
};

// Structures
typedef struct __rule {
    int head[MAX_ATOM_LENGTH];
    int body[MAX_ATOM_LENGTH];
    int head_length;
    int body_length;
    RULE_TYPE type;
}_rule;

typedef struct __literals {
    int atoms[MAX_ATOM_LENGTH];
    int atoms_length;    
}_literals;


#endif



