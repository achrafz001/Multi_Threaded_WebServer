#include <pthread.h>
#include "request.h"
#include "thread_master.h"
#include "io_helper.h"
#include <stdio.h>
#include <string.h>

// Definition of the locks and conditions
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


int end = -1;
int start = 0;
int size = 0;

// Function of the thread master
void *thread_master (void *args){

// creation of thread workers
	printf("end of the creation of thred workers\n");
	struct master *input = (void *)args;
	printf(" number of threads :%d\n",input->n_thr);
	printf(" maximum number of requests :%d\n",input->b);
	for(int i=0;i<input->n_thr;i++){
			    pthread_create(&input->pool[i], NULL,thread_worker, NULL);
			}
	printf("End of the creation of thread workers\n");
	buffersize = 0;
	int listen_fd = open_listen_fd_or_die(input->port);

	while (1) {   listen_function (listen_fd); } // still listen to the port 

	return 0;
}

// listen to the port chosen by the user
void listen_function (int listen_fd) {
	    struct stat sbuf;
	    char filename[MAXBUF];
	    
	    struct sockaddr_in client_addr;
	    int client_len = sizeof(client_addr);
	    int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
	    
            pthread_mutex_lock(&lock);             // lock to see if there is space to stock another request
            //adding requests to buffer
            int bool=stock(conn_fd,filename,sbuf.st_size);
            while(bool==-1)
            {
              bool=stock(conn_fd,filename,sbuf.st_size);
            }
            //mentionning the presence of at least one request into the buffer
           pthread_cond_signal(&cond);
           pthread_mutex_unlock(&lock);
}

void *thread_worker (void *arg)
{
  while (1)
  {
     //removing  a request from buffer
      Request req;
      pthread_mutex_lock(&lock);                // lock to see if there is requests or not 
      req=remove_fun();
      while(req.filedes == -1)
      {
        pthread_cond_wait(&cond, &lock);
        req = remove_fun();
      }
      pthread_mutex_unlock(&lock);
      request_handle(req.filedes);
      close_or_die(req.filedes);

   }
}

// function to stock request in the Buffer
int stock(int fd,char *fn,int fs)
{
  if (size == max_buffer)
    return -1;
  end = (end + 1) % max_buffer;
  Buffer[end].filedes = fd;
  Buffer[end].filename=fn;
  Buffer[end].filesize=fs;
  printf("Request for %s is added to the buffer.\n", fn);
  size++;
  return 1;
}

// function to remove request from the Buffer
Request remove_fun()
{
  Request req;
  req.filedes=-1;
  req.filename=NULL;
  req.filesize=-1;
  if (size == 0)
    return req;
  req.filedes= Buffer[start].filedes;
  req.filename=Buffer[start].filename;
  req.filesize=Buffer[start].filesize;
  printf("Request for %s is removed from the buffer.\n", req.filename);
  start = (start + 1) % max_buffer;
  size--;
  return req;
}










