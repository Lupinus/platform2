#include "shmdata.h"
#include "structure.h"
struct shared_use_st * get_shared(struct shared_use_st * st, int shmid) {
    key_t uni_id=ftok(ftok_filepath,ftok_proj_id);
    printf("uni_id:%u\n",uni_id);
    extern int errno;
    errno=0;
    int running = 1;   
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];//用于保存输入的文本
    
    //printf("shmid:%u\n",shmget((key_t)uni_id, sizeof(struct shared_use_st), IPC_CREAT));
    //shmid = shmget((key_t)uni_id, sizeof(struct shared_use_st), IPC_EXCL|IPC_CREAT);
    shmid = shmget((key_t)uni_id, sizeof(struct shared_use_st), 0640|IPC_CREAT);
     
    if(errno!=0)
    {      
        perror("shmget failed");
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    if(shmid < 0)
    {   
        shmctl((key_t)uni_id,IPC_RMID,(struct shared_use_st*)st);
        shmid = shmget((key_t)uni_id, sizeof(struct shared_use_st), 0640|IPC_CREAT);
        fprintf(stderr, "shmget failed\n");
        //shmctl(shmid,IPC_RMID,(struct shmid_ds*)shared);
        //exit(EXIT_FAILURE);
    }   //将共享内存连接到当前进程的地址空间
    
    shm = shmat(shmid, (void*)0, 0);
    if(shm == (void*)-1)
    {  
        //fprintf(stderr, "shmat failed\n"); 
        perror("shmat");
        //shmctl(shmid,IPC_RMID,(struct shmid_ds*)shared);    
        exit(EXIT_FAILURE);
    }
   return (struct shard_use_st*)shm;  
}


struct shared_use_st * shard_init(void* handle, struct shared_use_st* st, int shmid) {
    // struct shared_use_st * st1;
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    st = get_shared(st, shmid);
    int portid, queue, ring;
    for(portid = 0;portid != ctx->port_num;++portid){
		uint8_t t = ctx->port_list[portid];
		for(ring = 0;ring != ctx->port_ringnum[t];++ring){
            st->after_ring[t][ring] = 0;
            st->before_ring[t][ring] = 0;
            st->count_ring[t][ring] = 0;
        }
        for(queue = 0; queue < ctx->port_queuenum[t]; queue++) {
            st->eth_queue[t][queue] = 0;
        }
        st->port_num = ctx->port_num;
        st->ring_num[t] = ctx->port_ringnum[t];
        st->queue_num[t] = ctx->port_queuenum[t]; 
    }

    return st;

}