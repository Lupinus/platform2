#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <malloc.h>
#include "pcapTest.h"
#include "dpdkTest.h"

#define MAX_NIC_NUM 20
char *conf[MAX_NIC_NUM + 3]; // entries of the configuration file named "initial.xml"
int conf_num;                // number of entries of the configuration file named "initial.xml"

int getConf(char *conf[], const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Wrong initial.xml path\n");
        return 0;
    }
    char c;
    int count = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        // char number between > and <
        if (c == '>')
        {
            int tmp = 0;
            if ((c = fgetc(fp)) == '\r')
                continue;
            tmp++;
            while ((c = fgetc(fp)) != '<')
                tmp++;

            fseek(fp, -tmp - 1, SEEK_CUR);
            conf[count] = (char *)malloc(sizeof(char) * tmp);
            int i;
            for (i = 0; i < tmp; i++)
                conf[count][i] = fgetc(fp);
            count++;
        }
    }
    return count;
}
int main(int argc, char *argv[])
{
    conf_num = getConf(conf, "../../conf/initial.xml");
    if (conf_num == 0)
    {
        printf("something went wrong about parsing initial.xml\n");
        return 0;
    }
    if (strcmp(conf[0], "pcap") == 0)
    {
        libpcap_test(conf, conf_num);
    }
    else if (strcmp(conf[0], "dpdk") == 0)
    {
        dpdk_test(conf, conf_num);
    }
    else
    {
        printf("wrong configuration\n");
        return -1;
    }
}
