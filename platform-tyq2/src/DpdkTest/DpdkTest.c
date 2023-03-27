#define _GNU_SOURCE
#include "mydaqc.h"
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <regex.h>

#include <rte_mbuf.h>
#include <sys/time.h>
#include <time.h>

#define _THREADNUM_ 1
/*************************
这个文件是一个释放线程
 *************************/

#define NUM 128
// #define NUM 1024

#define NEW_TIME_VALE
struct timeval startTime, endTime;

float Timeuse;

#define START_GETTIME gettimeofday(&startTime, NULL);

#define END_GETTIME                                                                                  \
    gettimeofday(&endTime, NULL);                                                                    \
    Timeuse = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec); \
    Timeuse /= 1000000;                                                                              \
    printf("\nTimeuse = %f s\n", Timeuse);

struct myDaq *daq;
// int Thread_num = 2;
int timeflag = 1;
pthread_t ntid[10];
int packetNUM[NUM + 1];
int end;
pcap_dumper_t *dumper = NULL;
pthread_mutex_t mutexDumper;
char **conf;
int confNum;
void dumpFile(const u_char *pkt, int len, time_t tv_sec, suseconds_t tv_usec)
{
    struct pcap_pkthdr hdr;
    hdr.ts.tv_sec = tv_sec;
    hdr.ts.tv_usec = tv_usec;
    hdr.caplen = len;
    hdr.len = len;

    pcap_dump((u_char *)dumper, &hdr, pkt);
}

void dpdk_finish(int signum)
{
    printf("\n");
    timeflag = 0;
    int i;
    for (i = 0; i < Thread_Num; i++)
    {
        pthread_join(ntid[i], NULL);
    }

    pcap_dump_close(dumper);
    pthread_mutex_destroy(&mutexDumper);
    statData(daq);
    exit(1);
}

void sendQueue2(void *v)
{

    // sleep(1);
    unsigned int thread_num = *(int *)v;
    free(v);

    void **mbuf2 = (void **)malloc(3000 * sizeof(void *));
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(end + 1 + thread_num, &mask);
    pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);

    if (mbuf2 == NULL)
    {
        printf("Error at malloc mbuf");
        exit(0);
    }
    int *card;

    int count = 0;

    while (timeflag)
    {
        int num_get = masterRx(daq, mbuf2, NUM, thread_num, card);
        count += num_get;
        int i;
        for (i = 0; i < num_get; i++)
        {
            if (strcmp(conf[1], "1") == 0)
            {
                struct timeval tv;
                gettimeofday(&tv, NULL);
                char *pktbuf = rte_pktmbuf_mtod((struct rte_mbuf *)(mbuf2[i]), char *);
                pthread_mutex_lock(&mutexDumper);
                dumpFile((const u_char *)pktbuf, ((struct rte_mbuf *)(mbuf2[i]))->data_len, tv.tv_sec, tv.tv_usec);
                pthread_mutex_unlock(&mutexDumper);
                daq_free(daq, mbuf2[i]);
            }
            packetNUM[num_get]++;
        }
    }
}
int initDaq_DPDK(struct myDaq *daq, const char *path)
{
    void *dp = dlopen(path, RTLD_LAZY);
    if (dp == NULL)
    {
        printf("Error in read %s.\n", path);
        fputs(dlerror(), stderr);
        return -1;
    }
    struct myDaq *dm;
    dm = (struct myDaq *)dlsym(dp, "DAQ_MODULE_DATA");
    if (dm == NULL)
    {
        printf("Error in load data.\n");
        return -2;
    }
    memcpy(daq, dm, sizeof(struct myDaq));

    int core_start = 0;
    int core_end = 0;
    init(daq, Thread_Num, &core_start, &core_end);

    // printf("-----%d---%d--()()()()(--\n", core_start, core_end);
    end = core_end;
    printf("Done init.\n");
    return 0;
}

void begin(void)
{
    daq = (struct myDaq *)malloc(sizeof(struct myDaq));
    if (daq == NULL)
    {
        printf("Error in apply memeroy for daq.\n");
        return;
    }

    //目前这个函数中，dpdk的初始化参数都是写死的。是 "-c ff -n 4 -- -p 3 -q 4"
    initDaq_DPDK(daq, "../src/CaptureDpdk/libCaptureDpdk.so"); //初始化daq，第一个参数是之前分配好的内存，第二个参数是mydpdk.so的路径。
    /*以上代码为初始化dpdk的代码。整个程序中只能且必须运行一次。*/
}
void dpdk_test(char **conf, int confNum)
{
    if (strcmp(conf[1], "1") == 0)
    {
        dumper = pcap_dump_open(pcap_open_dead(DLT_EN10MB, 1600), "DpdkTest.pcap");
        if (NULL == dumper)
        {
            printf("dumper is NULL\n");
            return;
        }
    }
    /*以下代码为初始化dpdk的代码。整个程序中只能且必须运行一次。*/
    //为daq结构体分配一段内存，用来保存daq的信息。
    begin();

    pthread_mutex_init(&mutexDumper, NULL);
    int *tid;
    int i;
    for (i = 0; i < Thread_Num; i++)
    {
        tid = (int *)malloc(sizeof(int));
        *tid = i;

        pthread_create(&ntid[i], NULL, (void *)sendQueue2, tid);
        sleep(1);
    }
    printf("-----------start timecounting-------------\n");
    NEW_TIME_VALE
    START_GETTIME
    signal(SIGINT, dpdk_finish);
    while (1)
    {
        sleep(1);
    }
}
