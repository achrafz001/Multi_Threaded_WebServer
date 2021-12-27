#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include "thread_master.h"
char default_root[] = ".";


//
// ./wserver [-d <basedir>] [-p <portnum>]
//


int main(int argc, char *argv[]) {

    int c;
    char *root_dir = default_root;
    int port = 10000;
 // default values 
    num_threads= DEFAULT_THREADS;
    max_buffer=DEFAULT_BUFFER_TAILLE;

    pthread_t Thread_Master;


    while ((c = getopt(argc, argv, "d:p:t:b:")) != -1)
        switch (c) {
        case 'd':
            root_dir = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 't':
                    num_threads = atoi(optarg);
                    break;
        case 'b':
                    max_buffer = atoi(optarg);
                            break;
        default:
            fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t num_threads] [-b max_buffer] \n");
            exit(1);
        }
    	pthread_t pool[num_threads];
         struct master input;
            input.n_thr=num_threads;
            input.b=max_buffer;
            input.pool= &pool;
            input.port=port;

    // run out of this directory
    chdir_or_die(root_dir);
    pthread_create(&Thread_Master,NULL,thread_master,&input);  // creation of thread master
    pthread_join(&Thread_Master,NULL);




    return 0;
}

