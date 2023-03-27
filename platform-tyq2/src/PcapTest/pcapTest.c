#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>
#include "pcapHash.h"
#include "pcap.h"

#define MAX_NIC_Num 20
char **conf;
int confNum;
int NIC_Num;
pthread_t pts[MAX_NIC_Num];
struct my_hash *hash;
int packet_num_tot;
pthread_mutex_t mutexPacketNum;
pthread_mutex_t mutexDumper;
pcap_dumper_t *dumper = NULL;

void _capturePacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int *packet_num = (int *)arg;
    (*packet_num)++;
    if (strcmp(conf[1], "1") == 0)
    {
        pthread_mutex_lock(&mutexDumper);
        pcap_dump((u_char *)dumper, pkthdr, packet);
        pthread_mutex_unlock(&mutexDumper);
    }
}
void captureFinish(int signum)
{
    pcap_t *device = findDevice(pthread_self());
    if (device)
    {
        pcap_breakloop(device);
        pcap_close(device);
    }
}
void *capturePacket(void *arg)
{
    char *dev = (char *)arg;
    char errbuf[1024];
    pcap_t *device = pcap_open_live(dev, 65535, 1, 0, errbuf);
    if (strcmp(conf[1], "1") == 0)
    {
        dumper = pcap_dump_open(pcap_open_dead(DLT_EN10MB, 1600), "PcapTest.pcap");
        if (NULL == dumper)
        {
            printf("dumper is NULL\n");
            return;
        }
    }

    if (!device)
    {
        printf("couldn't open the net device: %s\n", errbuf);
        return NULL;
    }
    addThreadDevice(pthread_self(), device);
    int packet_num = 0;
    pcap_loop(device, -1, _capturePacket, (u_char *)(&packet_num));
    printf("NIC %s received %d packets\n", dev, packet_num);
    pthread_mutex_lock(&mutexPacketNum);
    packet_num_tot += packet_num;
    pthread_mutex_unlock(&mutexPacketNum);
    return NULL;
}

void finish()
{
    printf("\n");
    int i;
    for (i = 0; i < NIC_Num; i++)
        pthread_kill(pts[i], SIGUSR1);

    // for (i = 0; i < confNum; i++)
    //     free(conf[i]);

    for (i = 0; i < NIC_Num; i++)
        pthread_join(pts[i], NULL);

    pcap_dump_close(dumper);
    pthread_mutex_destroy(&mutexPacketNum);
    pthread_mutex_destroy(&mutexDumper);
    printf("totally received %d packets\n", packet_num_tot);
    printf("exit\n");
    exit(0);
}
void libpcap_test(char **_conf, int _confNum)
{
    conf = _conf;
    confNum = _confNum;
    if (confNum <= 2)
    {
        printf("wrong configuration\n");
        return;
    }
    NIC_Num = atoi(conf[2]);

    pthread_mutex_init(&mutexPacketNum, NULL);
    pthread_mutex_init(&mutexDumper, NULL);
    int i;
    for (i = 0; i < NIC_Num; i++)
    {
        if (pthread_create(&pts[i], NULL, capturePacket, conf[i + 3]))
        {
            printf("pthread creation fail\n");
            exit(1);
        }
        printf("NIC %s is capturing packet\n", conf[i + 3]);
    }
    signal(SIGINT, finish);
    signal(SIGUSR1, captureFinish);
    while (1)
    {
        sleep(1);
    }
}