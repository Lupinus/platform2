#ifndef __LIBPCAP_HASH_H
#define __LIBPCAP_HASH_H
#include <pthread.h>
#include <pcap.h>
#include "uthash.h"
typedef struct
{
    pthread_t t;
    pcap_t *device;
    UT_hash_handle hh;
} pcap_hash;
pcap_hash *p_hash = NULL;

void addThreadDevice(pthread_t t, pcap_t *device)
{
    pcap_hash *s;

    s = (pcap_hash *)malloc(sizeof(pcap_hash));
    s->t = t;
    s->device = device;
    HASH_ADD_INT(p_hash, t, s);
}

pcap_t *findDevice(pthread_t t)
{
    pcap_hash *s = NULL;
    HASH_FIND_INT(p_hash, &t, s);

    if (s != NULL)
    {
        return s->device;
    }
    else
    {
        return NULL;
    }
}
// // add an element,the key type is pthread_t,and value type is pcap_t(a device NIC),short for t_d
// void add_t_d(pthread_t, pcap_t *);
// // find a device via thread id
// pcap_t *find_d(pthread_t);
#endif
