#ifndef _STRUCTURE_H
#define _STRUCTURE_H

#define BURST_SIZE 64
#define MAX_CORE_SIZE 80
#define PORT_SIZE 16
#define CORE_SIZE 1024
#define QUEUE_SIZE 80
#include <stdint.h>
#include <rte_ring.h>
#include <rte_common.h>
#include <rte_errno.h>
#include <rte_byteorder.h>
#include <rte_log.h>
#include <rte_debug.h>
#include <rte_cycles.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_atomic.h>
#include <rte_branch_prediction.h>
#include <rte_mempool.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>
#include <rte_interrupts.h>
#include <rte_pci.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_dev.h>
#include <rte_string_fns.h>

#ifndef DAQ_SO_PUBLIC
#if defined _WIN32 || defined __CYGWIN__
#if defined DAQ_DLL
#ifdef __GNUC__
#define DAQ_SO_PUBLIC __attribute__((dllexport))
#else
#define DAQ_SO_PUBLIC __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define DAQ_SO_PUBLIC __attribute__((dllimport))
#else
#define DAQ_SO_PUBLIC __declspec(dllimport)
#endif
#endif
#define DLL_LOCAL
#else
#ifdef HAVE_VISIBILITY
#define DAQ_SO_PUBLIC __attribute__((visibility("default")))
#define DAQ_SO_PUBLIC __attribute__((visibility("hidden")))
#else
#define DAQ_SO_PUBLIC
#define DAQ_SO_PUBLIC
#endif
#endif
#endif

struct myDaq
{
	void *handle;
	int (*_init)(void **handle, int, int *, int *);
	int (*_masterRx)(void *, void **, int, int, int *);
	int (*_masterTx_a)(void *handle, void **, int, int);
	void *(*_daq_pktbuf_malloc)(void *, void **);
	int (*_daq_free)(void *);
	int (*_getPktHead)(void *, void *, void **);
	int (*_setPktlen)(void *, void *, int);
	int (*_statData)(void *);
	// int (*_portNum)(void *);
};

struct dpdk_context_t
{
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports;
	uint64_t rxsize[1024], txsize[1024];
	uint64_t before_enqueue[16][16], after_enqueue[16][16], after_dequeue[16][16];
	uint64_t rx_queue[16][16];
	uint64_t SleepNum, noSleepNum;
	uint64_t packetNum[BURST_SIZE * 2 + 1];
	struct rte_ring *txring[PORT_SIZE][QUEUE_SIZE];
	struct rte_ring *rxring[PORT_SIZE][QUEUE_SIZE];
	struct rte_eth_conf port_conf_default;
	uint32_t enabled_port_mask;
	// uint32_t queue_per_lcore;
	// uint32_t ring_per_lcore;
	int64_t timer_period;
	int stop;
	uint16_t port_list[PORT_SIZE];
	uint16_t port_num;
	int breakloop;
	//&&&
	int queue_sum;

	struct shared_use_st *st;
	int shmid;

	uint16_t start, end;
	unsigned thread_mode;
	unsigned queue_mode;
	unsigned ring_num;
	uint16_t port_ringnum[PORT_SIZE];
	uint16_t port_queuenum[PORT_SIZE];
	uint16_t core_portnum[CORE_SIZE];
	uint16_t core_queuenum[CORE_SIZE];
	uint16_t thread_portnum[MAX_CORE_SIZE];
	uint16_t thread_ringnum[MAX_CORE_SIZE];
};

typedef struct port_sort
{
	int Speed;
	uint8_t port_number;
	struct port_sort *next;
} port_sort;

port_sort *creatNode(int speed, int port_id);

int addNode(port_sort *sort, int speed, int port_id);

#endif
