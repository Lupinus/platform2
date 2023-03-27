#ifndef MYDAQC_H
#define MYDAQC_H
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <pcap/pcap.h>
#include <unistd.h>
//#include "tinyxml2.h"
#define MAX_ARGC 12

#define Thread_Num 1
// #ifndef DAQ_SO_PUBLIC
// #if defined _WIN32 || defined __CYGWIN__
// #  if defined DAQ_DLL
// #    ifdef __GNUC__
// #      define DAQ_SO_PUBLIC __attribute__((dllexport))
// #    else
// #      define DAQ_SO_PUBLIC __declspec(dllexport)
// #    endif
// #  else
// #    ifdef __GNUC__
// #      define DAQ_SO_PUBLIC __attribute__((dllimport))
// #    else
// #      define DAQ_SO_PUBLIC __declspec(dllimport)
// #    endif
// #  endif
// #  define DLL_LOCAL
// #else
// #  ifdef HAVE_VISIBILITY
// #    define DAQ_SO_PUBLIC  __attribute__ ((visibility("default")))
// #    define DAQ_SO_PUBLIC __attribute__ ((visibility("hidden")))
// #  else
// #    define DAQ_SO_PUBLIC
// #    define DAQ_SO_PUBLIC
// #  endif
// #endif
// #endif

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
//&&&

void *daq_pktbuf_malloc(struct myDaq *daq, void **addr);
/**
 *Set the packet length to send.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param pkt
 *   The packet mbuf's address.(Notice Is the previously assigned MBUF address, not the actual address of the packet)
 * @param len
 * 	 The packet actual length
 */
int setPktlen(struct myDaq *daq, void *pkt, int len);
/**
 *Retrive the packets from the choise port.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param bufs
 *   The packets' address recived will be record here.
 * @param num
 *   The number of packets you want to retrive.
 * @param port
 *   The id of port you want to get packets from.
 */
int masterRx(struct myDaq *daq, void **bufs, int num, int thread_num, int *card_index);
/**
 *Send the packets from the choise port.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param bufs
 *   The packets' address will be record here.
 * @param nb_mbuf
 *   The number of packets you want to send.
 * @param thread_num
 *   The id of port you want to send packets to.
 */
int masterTx_a(struct myDaq *daq, void **bufs, int nb_mbuf, int thread_num);
// int phaseArgv(struct myDaq * daq,char *path = "./conf.xml");
/**
 *Show state of daq.
 *
 *@param daq
 *	A point to daq basic content.
 */
void showState(struct myDaq *daq);
/**
 *Init daq and record some basic content.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param argc
 *   The argc argument that was given to the main() function.
 * @param argv
 *   The argv argument that was given to the main() function.
 * @return
 *   - On success, return 0.
 *   - On failure, the func will end the program.
 */
int init(struct myDaq *daq, int thread_num, int *core_start, int *core_end);

/**
 *Send the packets from one port of the choise port.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param bufs
 *   The packets you want to send.
 * @param nb_mbuf
 *   The number of packets you want to send.
 * @param p
 *   The id of port you want to send packets to.
 * @param t
 *   The thread id of caller. The func will decide send the packets to which queue of the port according this param.
 */
void masterTx(struct myDaq *daq, void **bufs, int nb_mbuf, int p, int t);

/**
 *Free space of a packet. Now have bugs.
 *
 *@param daq
 *	 A point to daq basic content.
 * @param m
 *   The packets' true address.
 */
int daq_free(struct myDaq *daq, void *m);
int statData(struct myDaq *daq);
int getPktHead(struct myDaq *daq, void *m, void **pkt);
int portNum(struct myDaq *daq);
// {
// 	/* data */
// };

#endif
// end of mydaqc.h
