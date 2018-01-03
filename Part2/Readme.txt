Part 2 - Signal Handling in Linux

Steps to execute:-

1) Generate the user app by compiling the files using make
make

2) run the executable in the terminal
./precisemain

3) Select an option inorder to check different tasks.

4) End the app using ctrl + c.

Steps to run kernelshark:-

1) gcc -o precisemain.c -o precisemain.o -lpthread -lm
2) trace-cmd record -e sched_switch -e signal taskset -c 0 ./precisemain.o
3) kernelshark
4) Select from lists to view only CPU0 and precisemain.o tasks.