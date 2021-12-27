#ifndef __THREAD_MASTER__
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "io_helper.h"

#define MAXBUF (8192)
#define TAILLE (1000)

// structure contains different inputs to give it to the thread master
struct master{
	int n_thr;       // num_threads
	int b;           // max
	int port;
	pthread_t *pool;       //pool of threads worker
};

// Structure that contains caracteristics of the request entered from the client
typedef struct request{
  int filedes;
  char *filename;
  int filesize;
}Request;

// Buffer of requests : used to stock and remove requests 
Request Buffer[TAILLE];

// Different parameters used to stock and remove requests (used in wserver.c and thread_master.c)
int max_buffer;
int buffersize;
int num_threads;

// function to stock request in the Buffer
int stock(int fd,char *fn,int fs);
Request remove_fun();       // function to remove request from the Buffer

void *thread_master(void *args);   // Declaration of the tread master function to use it in wserver.c 

void *thread_worker(void *arg);  // Declaration of the tread worker function 

#endif //__THREAD_MASTER__
