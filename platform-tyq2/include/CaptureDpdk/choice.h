#ifndef _CHONICE_H
#define _CHONICE_H
#include <stdio.h>
#include "structure.h"
#include "json.h"

unsigned get_port_speed(uint8_t port_id) ;

/*Different number of rings are allocated on different network cards*/
int get_port_ringnum(void* handle) ;
/*Create different numbers of threads according to the number of rings*/
int bind_core_length(void* handle);

/*Bind the corresponding network card number to different cores*/
int get_core_portnum(void* handle);

/*Get the number of queues corresponding to the network card*/
int get_port_queuenum(void* handle);

/*Get the network card number corresponding to the second layer processing thread*/
int get_thread_portinfo(void* handle);


int choice_init(void* handle);

#endif