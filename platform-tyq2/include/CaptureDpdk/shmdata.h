#ifndef _SHMDATA_H
#define _SHMDATA_H

#define TEXT_SZ 2048
#define ftok_filepath "./dpdk.c"
#define ftok_proj_id 0666
#include "stdint.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>

struct shared_use_st
{  
    //int written;//作为一个标志，非0：表示可读，0表示可写
    // char text[TEXT_SZ];//记录写入和读取的文本
    int port_num;
    int queue_num[16];
    int ring_num[16];
    uint64_t before_ring[16][80];
    uint64_t count_ring[16][80];
    uint64_t after_ring[16][80];
    uint64_t eth_queue[16][80];
};


struct shared_use_st * get_shared(struct shared_use_st * st, int shmid);

struct shared_use_st * shard_init(void* handle, struct shared_use_st* st, int shmid);
#endif