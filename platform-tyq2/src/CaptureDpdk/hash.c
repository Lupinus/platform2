#include "hash.h"
Hash *hash = NULL;
// 增加
void add(int key, double value1, int value2)
{
    //Hash *s = NULL;
    Hash *s;
    s = (Hash *)malloc(sizeof(Hash));
    s->key = key;
    s->value1_weight = value1;
    s->value2_queue_num = value2;
    HASH_ADD_INT(hash, key, s); 
}

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
double find_weight(int key)
{
    Hash *s = NULL;
    HASH_FIND_INT(hash, &key, s);
    
    if (s != NULL) {
        // 查找到结果
        return s->value1_weight;
    } else {
        return -1;
    }
}
// 查找
int find_queue(int key)
{
    Hash *s = NULL;
    HASH_FIND_INT(hash, &key, s);
    
    if (s != NULL) {
        // 查找到结果
        return s->value2_queue_num;
    } else {
        return -1;
    }
}


// // 删除
// void delete (Hash *s)
// {
//     HASH_DEL(hash, s);
//     free(s);
//     s = NULL;
// }

