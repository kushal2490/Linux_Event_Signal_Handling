TASK 3 - Signal delivery to all threads of a process

Part 1

Files:-
async_signal.c
Makefile

Explanation:-
1) The main thread creates 5 pthreads which are unblocked to handle the SIGIO signal generated from the mouse event. Not all the threads registered to handle the signal together, any one would be picked.
2) We use fcntl() to make that particular running thread as the owner of the mouse device file inorder to handle the SIGIO signal of the mouse.
3) SIGIO will be handled arbitrarily by any of the 5 threads as we see it in the output.
---------------------------------------------------------------------
Part 2

Files:-
signaling_main.c
lib_signaling.h
Makefile

Explanation:-
1) In this the main thread creates 6 threads. 5 threads are used to register to thread with the library "lib_signalling.h" inorder to handle the SIGIO signal generated on a mouse event.
2) The threads which register with the mouse event thread handler using allocate_thread() can only receive the SIGIO mouse signal.
3) We register the thread with its pthread_id along with thread function. When the signal handler is invoked for a particular pthread, its corresponding thread_signalhandler prints out the handling of the event.
---------------------------------------------------------------------------------------------------------------


Steps to execute:-
Part 1

1) Generate the user app by compiling the files using make
make

2) run the executable in the terminal
./async_app
------------------------------------------------------------
Part 2

1) Generate the user app by compiling the files using make
make

2) run the executable in the terminal
./sig_app
------------------------------------------------------------
======================================================================