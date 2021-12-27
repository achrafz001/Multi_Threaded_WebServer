# Multi_Threaded_WebServer-
Academic project
The simplest approach to building a multi-threaded server is to spawn a new
thread for every new http request. The OS will then schedule these threads
according to its own policy. The advantage of creating these threads is that
now short requests will not need to wait for a long request to complete;
further, when one thread is blocked (i.e., waiting for disk I/O to finish) the
other threads can continue to handle other requests. However, the drawback of
the one-thread-per-request approach is that the web server pays the overhead
of creating a new thread on every request.
Therefore, the generally preferred approach for a multi-threaded server is to
create a fixed-size pool of worker threads when the web server is first
started. With the pool-of-threads approach, each thread is blocked until there
is an http request for it to handle. Therefore, if there are more worker
threads than active requests, then some of the threads will be blocked,
waiting for new HTTP requests to arrive; if there are more requests than
worker threads, then those requests will need to be buffered until there is a
ready thread.
In your implementation, you must have a master thread that begins by creating
a pool of worker threads, the number of which is specified on the command
line. Your master thread is then responsible for accepting new HTTP
connections over the network and placing the descriptor for this connection
into a fixed-size buffer; in your basic implementation, the master thread
should not read from this connection. The number of elements in the buffer is
also specified on the command line. Note that the existing web server has a
single thread that accepts a connection and then immediately handles the
connection; in your web server, this thread should place the connection
descriptor into a fixed-size buffer and return to accepting more connections.
Each worker thread is able to handle both static and dynamic requests. A
worker thread wakes when there is an HTTP request in the queue; when there are
multiple HTTP requests available, which request is handled depends upon the
scheduling policy, described below. Once the worker thread wakes, it performs
the read on the network descriptor, obtains the specified content (by either
reading the static file or executing the CGI process), and then returns the
content to the client by writing to the descriptor. The worker thread then
waits for another HTTP request.
Note that the master thread and the worker threads are in a producer-consumer
relationship and require that their accesses to the shared buffer be
synchronized. Specifically, the master thread must block and wait if the
buffer is full; a worker thread must wait if the buffer is empty. In this
project, you are required to use condition variables. Note: if your
implementation performs any busy-waiting (or spin-waiting) instead, you will
be penalized (i.e., do not do that!).
Side note: do not be confused by the fact that the basic web server we provide
forks a new process for each CGI process that it runs. Although, in a very
limited sense, the web server does use multiple processes, it never handles
more than a single request at a time; the parent process in the web server
explicitly waits for the child CGI process to complete before continuing and
accepting more HTTP requests. When making your server multi-threaded, you
should not modify this section of the code.

Security Considerations
Running a networked server can be dangerous, especially if you are not
careful. Thus, security is something you should consider carefully when
creating a web server. One thing you should always make sure to do is not
leave your server running beyond testing, thus leaving open a potential
backdoor into files in your system.
Your system should also make sure to constrain file requests to stay within
the sub-tree of the file system hierarchy, rooted at the base working
directory that the server starts in. You must take steps to ensure that
pathnames that are passed in do not refer to files outside of this sub-tree.
One simple (perhaps overly conservative) way to do this is to reject any
pathname with .. in it, thus avoiding any traversals up the file system
tree. More sophisticated solutions could use chroot() or Linux containers,
but perhaps those are beyond the scope of the project.

Command-line Parameters
Your C program must be invoked exactly as follows:

prompt> ./wserver [-d basedir] [-p port] [-t threads] [-b buffers]


The command line arguments to your web server are to be interpreted as
follows.


basedir: this is the root directory from which the web server should
operate. The server should try to ensure that file accesses do not access
files above this directory in the file-system hierarchy. Default: current
working directory (e.g., .).

port: the port number that the web server should listen on; the basic web
server already handles this argument. Default: 10000.

threads: the number of worker threads that should be created within the web
server. Must be a positive integer. Default: 1.

buffers: the number of request connections that can be accepted at one
time. Must be a positive integer. Note that it is not an error for more or
less threads to be created than buffers. Default: 1.

For example, you could run your program as:

prompt> server -d . -p 8003 -t 8 -b 16


In this case, your web server will listen to port 8003, create 8 worker threads for
handling HTTP requests, and allocate 16 buffers for connections that are currently
in progress (or waiting).

Source Code Overview
We recommend understanding how the code that we gave you works.  We provide
the following files:


wserver.c: Contains main() for the web server and the basic serving loop.

request.c: Performs most of the work for handling requests in the basic
web server. Start at request_handle() and work through the logic from
there.

io_helper.h and io_helper.c: Contains wrapper functions for the system calls invoked by
the basic web server and client. The convention is to add _or_die to an
existing call to provide a version that either succeeds or exits. For
example, the open() system call is used to open a file, but can fail for a
number of reasons. The wrapper, open_or_die(), either successfully opens a
file or exists upon failure.

wclient.c: Contains main() and the support routines for the very simple
web client. To test your server, you may want to change this code so that it
can send simultaneous requests to your server. By launching wclient
multiple times, you can test how your server handles concurrent requests.

spin.c: A simple CGI program. Basically, it spins for a fixed amount
of time, which you may useful in testing various aspects of your server.

Makefile: We also provide you with a sample Makefile that creates
wserver, wclient, and spin.cgi. You can type make to create all of
these programs. You can type make clean to remove the object files and the
executables. You can type make server to create just the server program,
etc. As you create new files, you will need to add them to the Makefile.

The best way to learn about the code is to compile it and run it. Run the
server we gave you with your preferred web browser. Run this server with the
client code we gave you. You can even have the client code we gave you contact
any other server that speaks HTTP. Make small changes to the server code
(e.g., have it print out more debugging information) to see if you understand
how it works.
