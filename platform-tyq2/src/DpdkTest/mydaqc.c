#include <arpa/inet.h>
#include <netinet/ip.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define MAX_ARGC 12
#define MAXARGS 20
#include "mydaqc.h"

int end;
/*
void initMyDaq(struct myDaq * daq,const char * path){
	printf("Load dpdk from %s.\n",path);
	void * dp = dlopen(path,RTLD_LAZY);
	if(dp == NULL){
		printf("Error in read %s.\n",path);
		return;
	}
	daq->_init = (int (*)(void **,int,char **))dlsym(dp,"init");
	daq->_testTx = (void (*)(void *))dlsym(dp,"testTx1");
	daq->_masterRx = (void (*)(void *,void **,int,int))dlsym(dp,"masterRx");
	daq->_masterTx = (void (*)(void *,void **, int,int,int))dlsym(dp,"masterTx");
	daq->_testRxSpeed = (void (*)(void *))dlsym(dp,"testRxSpeed");
	daq->_kill = (void (*)(void *))dlsym(dp,"kill");
	daq->_showState = (void (*)(void *))dlsym(dp,"showState");
	daq->_getTime = (struct timeval (*)(void *))dlsym(dp,"getTime");
	daq->_daq_pktbuf_malloc = (void *(*)(void*,void **))dlsym(dp,"daq_pktbuf_malloc");
	daq->_daq_free = (void (*)(void *))dlsym(dp,"daq_free");
	daq->_showLog = (void (*)(FILE *))dlsym(dp,"showLog");
	daq->_masterTx_a = 	(void (*)(void *,void **, int,int))dlsym(dp,"masterTx_a");
	daq->_masterTx_d = 	(void (*)(void *,void **, int,int,int))dlsym(dp,"masterTx_direct");
	daq->_set = (int (*)(void *,int))dlsym(dp,"set");
	daq->_sendPacket = (void (*)(void *,void *,int,int))dlsym(dp,"sendPacket");
	printf("Done load dpdk.\n");
	//argc = phaseArgv("./test.xml");
	//if(argc ==0){
		char *argv[10] = {
			"dpdk","-c","ff","-n","4","--","-p","f","-q","4"
		};

	init(daq,10,argv);
	//}//else{
	//	std::cout << "Pharse Done." << std::endl;
	//	for(int i =0;i!=argc;++i){
	//		std::cout << i << ":" << argv[i] << std::endl;
	//	}
	//	init(daq,argc,argv);
	//}
	printf("Done init.Now test.\n");
}
*/

// int getArgs(char* args[],const char * path){
// 	FILE * fptr = fopen(path, "r");
// 	int idx = 0;
// 	if (fptr == NULL)
// 		return -1;//no file
// 	char str[8196];
// 	char match[100];
// 	char errbuf[1024];
// 	char * pattern = "<([^>]+)>([^<]+)</\\1>";
// 	regex_t reg;
// 	int err, nm = 10;
// 	regmatch_t pmatch[MAXARGS];
// 	int count = 0;
// 	if (regcomp(&reg, pattern, REG_EXTENDED) < 0){
// 		regerror(err, &reg, errbuf, sizeof(errbuf));
// 		printf("err:%s\n", errbuf);
// 	}

// 	while (fgets(str, 8195, fptr)){
// 		err = regexec(&reg, str, nm, pmatch, 0);
// 		if (err == REG_NOMATCH){
// 			//printf("Not match in %s.\n", str);
// 			continue;
// 		}
// 		else if (err){
// 			regerror(err, &reg, errbuf, sizeof(errbuf));
// 			//printf("err:%s\n", errbuf);
// 			continue;
// 		}
// 		int i;
// 		for (i = 2; i < MAXARGS&&pmatch[i].rm_so != -1; i++){
// 			int len = pmatch[i].rm_eo - pmatch[i].rm_so;
// 			if (len){
// 				memset(match, '\0', sizeof(match));
// 				memcpy(match, str + pmatch[i].rm_so, len);
// 				printf("%s\n", match);
//                 args[count] = (char *)malloc(sizeof(char)*(len+1));
// 				strcpy(args[count],match);
// 			}
// 		}
// 		count++;
// 	}
// 	return count;
// }

int setPktlen(struct myDaq *daq, void *pkt, int len)
{
	if (daq->_setPktlen == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return;
	}
	return daq->_setPktlen(daq->handle, pkt, len);
}
// void showState(struct myDaq * daq){
// 	if(daq->_showState==NULL){
// 		printf("Error in load func %s().\n",__FUNCTION__);
// 		return ;
// 	}
// 	return daq->_showState(daq->handle);
// }

int getPktHead(struct myDaq *daq, void *m, void **pkt)
{
	if (daq->_getPktHead == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return 0;
	}
	return daq->_getPktHead(daq->handle, m, pkt);
}

int init(struct myDaq *daq, int thread_num, int *core_start, int *core_end)
{
	if (daq->_init == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return -1;
	}
	return daq->_init(&daq->handle, thread_num, core_start, core_end);
}

int masterRx(struct myDaq *daq, void **bufs, int num, int thread_num, int *card_num)
{
	if (daq->_masterRx == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return 0;
	}
	return daq->_masterRx(daq->handle, bufs, num, thread_num, card_num);
}

// int masterRx1(struct myDaq * daq,void ** bufs,int num,int port){
// 	if(daq->_masterRx==NULL){
// 		printf("Error in load func %s().\n",__FUNCTION__);
// 		return ;
// 	}
// 	return daq->_masterRx1(daq->handle,bufs,num,port);
// }

int masterTx_a(struct myDaq *daq, void **bufs, int nb_mbuf, int p)
{
	if (daq->_masterTx_a == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return -1;
	}
	return daq->_masterTx_a(daq->handle, bufs, nb_mbuf, p);
}

void *daq_pktbuf_malloc(struct myDaq *daq, void **addr)
{
	if (daq->_daq_pktbuf_malloc == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return NULL;
	}
	return daq->_daq_pktbuf_malloc(daq->handle, addr);
}

int daq_free(struct myDaq *daq, void *m)
{
	if (daq->_daq_free == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return -1;
	}
	return daq->_daq_free(m);
}

//&&&
int statData(struct myDaq *daq)
{
	if (daq->_statData == NULL)
	{
		printf("Error in load func %s().\n", __FUNCTION__);
		return -1;
	}
	return daq->_statData(daq->handle);
}

// int portNum(struct myDaq * daq) {
// 	if(daq->_portNum == NULL){
// 		printf("Error in load fuc %s().\n",__FUNCTION__);
// 		return -1;
// 	}
// 	return daq->_portNum(daq->handle);

// }
// void memcp(struct myDaq * daq,void *pkt,void *mbuf,int size){
//    if(daq->_memcp == NULL){
//         printf("Error in load func %s.\n",__FUNCTION__);
//         return -1;
//    }
//    return daq->_memcp(daq->handle,pkt,mbuf,size);
// }