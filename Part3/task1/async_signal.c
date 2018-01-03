#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MOUSE_DEV 		"/dev/input/mice"


pthread_mutex_t mutex;
pthread_t mouse_thread_id[5];

/* Signal handler function to handle to display the pid of the thread handling the signal */
void signal_handler(int signo)
{
	printf("Signal handled by thread %u\n",(unsigned int) pthread_self());
	return;
}

/* Mouse Event handler thread to unblock any mouse event -> SIGIO  */
void* mouse_thread_fn(void* arg)
{
	struct sigaction sa;
	sigset_t mask;
	int mouse_fd;
	long oflags;
		
	mouse_fd = open(MOUSE_DEV, O_RDONLY);
	if(mouse_fd < 0)
	{
		printf("Cannot open Mouse Device\n");
	}

	memset(&sa, 0, sizeof(sa));
	sigfillset(&mask);
	sa.sa_handler = &signal_handler;
	sa.sa_mask = mask;
	sigaction(SIGIO, &sa, NULL);
	//unblock the signal for this thread
	pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

	//Using asynchronous i/o from mouse by making it as owner of the file
	fcntl(mouse_fd, F_SETOWN, getpid());
	oflags = fcntl(mouse_fd, F_GETFL);
	fcntl(mouse_fd, F_SETFL, oflags | FASYNC);

	while(1);
	pthread_exit(0);
}

int main()
{
	int mouse_fd, i;
	//long oflags;

	mouse_fd = open(MOUSE_DEV, O_RDONLY);
	if(mouse_fd < 0)
	{
		printf("Cannot open Mouse Device\n");
	}

	//Create multiple threads to handle signal arbitrarily
#if 1
	for(i=0; i<5; i++)
	{
		pthread_create(&mouse_thread_id[i], NULL, &mouse_thread_fn, NULL);
	}
	for(i=0; i<5; i++)
	{
		pthread_join(mouse_thread_id[i],NULL);
	}
#endif

return 0;
}