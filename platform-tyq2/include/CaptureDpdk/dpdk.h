#ifndef DPDK_H
#define DPDK_H
#include <stdbool.h>
#include <rte_malloc.h>
#include <rte_ip.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include <time.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <getopt.h>
//#include "mydaqc.h"
// //#include <rte_common.h>
// #include <rte_log.h>
// #include <rte_memory.h>
// #include <rte_memcpy.h>
// #include <rte_memzone.h>
// //#include <rte_eal.h>
// #include <rte_per_lcore.h>
// #include <rte_launch.h>
// #include <rte_atomic.h>
// #include <rte_cycles.h>
// //#include <rte_prefetch.h>
// #include <rte_lcore.h>
// #include <rte_per_lcore.h>
// #include <rte_branch_prediction.h>
// #include <rte_interrupts.h>
// #include <rte_pci.h>
// #include <rte_random.h>
// //#include <rte_debug.h>
// //#include <rte_ether.h>
// //#include <rte_ethdev.h>
// //#include <rte_ring.h>
// //#include <rte_mempool.h>
// //#include <rte_mbuf.h>

#include "structure.h"
#include "hash.h"
#include "shmdata.h"
#include "choice.h"

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024

#define NUM_MBUFS 2097151
#define MBUF_CACHE_SIZE 337

#define LOOP_ROUNDS 1024

#define TIMER_MILLISECOND 2000000ULL /* around 1ms at 2 Ghz */
#define MAX_TIMER_PERIOD 86400       /* 1 day max */
#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_RX_RING_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16
//#define showLog() rte_log_dump_history(stdout)
//&&&
#define QUEUE_MAX_SIZE 32
#define PKG_MAX_SIZE 104857600
#define PKG_RING_SIZE 4096

// mydaq
#define MAX_ARGC 12
#define MAXARGS 20

/*these functions shouldn't be called outside dpdk.c/dpdk.h*/
int initRing(struct rte_ring **r, const char *name, int ring_size, int socket_id);
inline int port_init(void *handle, uint8_t port, struct rte_mempool *mbuf_pool);
// int getRxRequest(void * handle);
// int getTxRequest(void * handle);
// void masterTx_infile(void * handle,struct rte_mbuf ** bufs, int nb_mbuf,int port_id,int th_num);

/*end of infile function*/
/*these functions can called outside this .so.*/
// init the dpdk, args can search the user guide of dpdk.
int initDaq(void **handle, int thread_num, int *core_start, int *core_end);
int dpdk_init(void **handle, int argc, char *argv[], int thead_num, int *Start, int *End);
int dpdk_port_num(void *handle);

// void ** bufs means the ptr point to the recieved packets' mem address
// int masterRx(void** bufs,int &nums);
// void masterRx(void *handle);
// int getPackets(void *handle, struct pcap_pkthdr **pkthdr ,u_char *pkts[], int nb_mbuf);

// recieved packets but discard them, just show the rx speed.
void dpdk_testRxSpeed(void *handle);
// send packets, it will rebuild the packets as rte_mbufs then called masterTx_infile.
// may have bugs, without test and do not know how to test.
// void masterTx_auto(void * handle,struct pcap_pkthdr **pkthdr ,u_char *pkts[], int nb_mbuf);
void dpdk_masterTx(void *handle, struct rte_mbuf **bufs, int nb_mbuf, int port_id, int th_id);
// void masterTx_ver1(void * handle,void ** _bufs, int nb_mbuf,int port_id,int th_num);
void dpdk_showLog(FILE *f);
void *dpdk_pktbuf_malloc(void *handle, void **addr);
int dpdk_set_pktlen(void *handle, void *m, int pktlen);
static inline int dpdk_free(void *m);
void dpdk_sendPacket(void *handle, void *_mbuf, int times, int port);
int dpdk_set(void *handle, int num);
// a test file, it will firstly recived some packets then send them.,.
void dpdk_testTx(void *handle);
void dpdk_kill(void *handle);
void dpdk_testTx1(void *handle);
struct timeval dpdk_getTime(void *handle);
void jiffies(void *handle);
int l2fwd_parse_args(void *handle, int argc, char **argv);
int main(int argc, char *argv[]);
int dpdk_getPktHead(void *handle, void *m, void **pkt);
void slaveCoreTx(void *handle);
void dpdk_masterTx_direct(void *handle, struct rte_mbuf **bufs, int nb_mbuf, int port_id, int queue_id);
int dpdk_masterRx(void *handle, struct rte_mbuf **bufs, int num, int thread_num, int *card_num);
int dpdk_masterRx_test1(void *handle, struct rte_mbuf **bufs, int num, int port_id);

int dpdk_masterTx_a(void *handle, struct rte_mbuf **bufs, int nb_mbuf, int port_id);
void dpdk_testSaveTraffic(void *handle);

void dpdk_testReadTraffic(void *handle);
static inline struct rte_mbuf *mcast_out_pkt(struct rte_mbuf *pkt, struct rte_mempool *mp);
void mainLoop(void *handle);
void *dpdk_lookupRing(char *name);
void *dpdk_ringEnqueue(void *rptr, void *buf);

int dpdk_ringDequeue(void *rptr, void **buf);
//&&&
void dpdk_recvpkg_pth(void *handle, int port);
void dpdk_distribute(void *handle, int queue_sum);
void dpdk_recvpkg(void *handle, int portsum, int pthreadsum);
int dpdk_getpkg_queue(void *handle, void ***pkg, int queue_num);
int dpdk_statis_data(void *handle);
void dpdk_clean_queue(void *handle, int queue_num, int cleansize);
void RandIP(char *s, int port);
void dpdk_quit(void);
/*end of api function*/
#endif
// end of dpdk.h
