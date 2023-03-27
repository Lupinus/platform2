#ifndef _HASH_H
#define _HASH_H
#include "uthash.h"
typedef struct {
    int key;
    double value1_weight;
    int value2_queue_num;
    UT_hash_handle hh;
} Hash;


// 增加
void add(int key, double value1, int value2);

// void add_port_id(int key, int value)
// {
//     //Hash *s = NULL;
//     Hash *s;
//     s = (Hash *)malloc(sizeof(Hash));
//     s->key = key;
//     s->port_id = value;
//     HASH_ADD_INT(hash, key, s); 
// }

// 查找
double find_weight(int key);
// 查找
int find_queue(int key);


// // 删除
// void delete (Hash *s)
// {
//     HASH_DEL(hash, s);
//     free(s);
//     s = NULL;
// }


#endif