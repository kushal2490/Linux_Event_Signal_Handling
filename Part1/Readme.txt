TASK 1 - Setjmp and longjmp

Files:-
imprecise.c
Makefile

Explanation:-
1) The main thread creates one thread to handle mouse events specifically the Right Double Click event.
2) This thread generates a signal SIGUSR1 using kill() system call on detection of a double click and this signal will be sent to the kernel. 
3) The main task saves the context using the setjmp() call and computes the sum as long as it receives the SIGUSR1 signal from the mouse event.
4) The kernel delivers this generated signal to the signal handler which then calls longjmp() inorder to transfer control back to the main task.
5) When the longjmp() restores the saved context of the main thread, it skips the calculation loop and displays the sum and time elapsed for the imprecise computation.

Steps to execute:-

Note:- Change the "/dev/input/event14" according to local desktop mouse.
This can be viewed by running the following command: 
cat /proc/bus/input/devices

Search for the Mouse brand name in use and find the event number corresponding to it and EDIT it MOUSE_DEV macro in this file.

1) generate the user app by compiling the files using make
make

2) run the executable in the terminal
./imprecise_app

======================================================================
