#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MOUSE_DEV "/dev/input/mice"

pthread_t thread_array[10];
pthread_mutex_t mutex;
static unsigned int count = 0;

void (*thread_func[10])();

/* This function registers threads to a signal handler inorder to receive a particular signal */
void allocate_thread(pthread_t thread, void* threadfunc)
{
	pthread_mutex_lock(&mutex);
	if(count < 5)
	{
		thread_func[count] = threadfunc;
		thread_array[count] = thread;
		count++;
	}
	pthread_mutex_unlock(&mutex);
}

/* Signal handler which calls a particular thread function attached to it */
void signal_handler(int signo)
{
	int i;
	for(i=0;i<count;i++)
	{
		thread_func[i]();
	}
}

/* Mouse event handler thread function */
void* signal_thread_func(void* arg)
{
	int mouse_fd;
	long oflags;
	sigset_t mask;
	struct sigaction sa;

	pthread_mutex_init(&mutex, NULL);

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

	pthread_mutex_destroy(&mutex);
	pthread_exit(0);

}