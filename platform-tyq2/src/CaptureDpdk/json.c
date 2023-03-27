#include "json.h"

int cJSON_to_struct_array(char *text, port_info worker[])  
{  
    cJSON *json,*arrayItem,*item,*object;  
    int i;  
    //struct dpdk_context_t * ctx = (struct dpdk_context_t *)handle;
    json=cJSON_Parse(text);  
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else 
    {   double weight = 0;
        int size = 0, speed = 0,queue_num = 0;
        arrayItem=cJSON_GetObjectItem(json,"port_info");  
        if(arrayItem!=NULL) {   
            size=cJSON_GetArraySize(arrayItem);
            for(i=0;i<size;i++)  
            {  
                printf("i=%d\n",i);  
                object=cJSON_GetArrayItem(arrayItem,i);  
 
 
                item=cJSON_GetObjectItem(object,"speed");  
                if(item!=NULL)  
                {  
                    printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%d\n",item->type,item->string,item->valueint);  
                    worker[i].speed=item->valueint;
                    speed = item->valueint;
                }  
                else 
                {  
                    printf("cJSON_GetObjectItem: get speed failed\n");  
                }

 
                item=cJSON_GetObjectItem(object,"weight");  
                if(item!=NULL)  
                {  
                    printf("cJSON_GetObjectItem: type=%d, string is %s, valuedouble=%f\n",item->type,item->string,item->valuedouble);  
                    worker[i].weight=item->valuedouble; 
                    weight = item->valuedouble; 
                }  
                else 
                {  
                    printf("cJSON_GetObjectItem: get weight failed\n");  
                }  

                item=cJSON_GetObjectItem(object,"queue_num");  
                if(item!=NULL)  
                {  
                    printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n",item->type,item->string,item->valueint);  
                    worker[i].queue_num=item->valueint;  
                    queue_num = item->valueint;
                }  
                else 
                {  
                    printf("cJSON_GetObjectItem: get queue_num failed\n");  
                }

                add(speed, weight, queue_num);
            }  
        }  
 
         for(i=0;i<size;i++)  
        {  
            printf("i=%d, speed=%d,weight=%f,queue_num = %d\n",  
                    i,  
                    worker[i].speed,    
                    worker[i].weight,
                    worker[i].queue_num);  
        }  
 
        cJSON_Delete(json);  
    }  
 return 0;  
}  
 
// Read a file, parse, render back, etc. 
void dofile(char *filename)  
{  
 FILE *f;  
 int len;  
 char *data;  
 
    f=fopen(filename,"rb");  
    fseek(f,0,SEEK_END);  
    len=ftell(f);  
    fseek(f,0,SEEK_SET);  
    data=(char*)malloc(len+1);  
    fread(data,1,len,f);  
    fclose(f);  
 
    printf("read file %s complete, len=%d.\n",filename,len);  

    port_info worker[3]={{0}}; 
 
    cJSON_to_struct_array(data, worker);  
 
    free(data);  
}  