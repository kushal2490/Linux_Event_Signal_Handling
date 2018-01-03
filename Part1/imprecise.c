#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <setjmp.h>
#include <linux/input.h>

#define MOUSE_DEV 		"/dev/input/event14"

jmp_buf env;

/* Mouse Event signal handler function to detect Double Right click event */
void* mouse_thread_fn(void* arg)
{
	int fd_event;
	int flag_click_1 = 0;
	long secs, usecs, msecs;
	struct input_event mouse_event;
	struct timeval first_click, second_click;

	fd_event = open(MOUSE_DEV, O_RDONLY);
	if(fd_event < 0)
	{
		printf("Error in opening Mouse Device File\n");
		return NULL;
	}
	// poll over the mouse device to capture Right Click mouse event
	while(read(fd_event, &mouse_event, sizeof(struct input_event)))
	{
		if(mouse_event.type == EV_KEY && mouse_event.code == BTN_RIGHT && mouse_event.value == 1)
		{
			//printf("TYPE = %04x, CODE = %04x, VALUE = %d\n",mouse_event.type, mouse_event.code, mouse_event.value);
			//Check for the first click
			if(flag_click_1 == 0)
			{
				gettimeofday(&first_click, NULL);
				flag_click_1 = 1;
				printf("Right Single Click\n");
			}
			//Check for the second click, if it is within 500ms delay then consider it as double click
			else
			{
				gettimeofday(&second_click, NULL);
				secs = second_click.tv_sec - first_click.tv_sec;
				usecs = second_click.tv_usec - first_click.tv_usec;
				msecs = (usecs/1000.0) + (secs*1000);
				if(msecs < 500)
				{
					printf("Right Double Click Event\n");
					//On successful capture of double click, generate signal to call long jump
					kill(0, SIGUSR1);
				}
				else
				{
					//if successive clicks > 500ms then treat as single click
					first_click = second_click;
					printf("Right Single Click\n");
				}
			}
		}
	}
return NULL;
}

/* signal handler function which calls longjump to return to initial set location set by setjump */
void signal_handler(int signo)
{
	longjmp(env, -1);
	return;
}

int main()
{
	struct sigaction sa;
	sigset_t mask;
	int sum = 0, i, j;
	int flag = 0;
	int ret;
	pthread_t mouse_thread_id;

	//Create mouse event handler thread
	pthread_create(&mouse_thread_id, NULL, &mouse_thread_fn, NULL);

	//mask the thread from all signal except SIGUSR1
	memset(&sa, 0, sizeof(sa));
	sigfillset(&mask);
	sa.sa_handler = &signal_handler;
	sa.sa_mask = mask;

	sigaction(SIGUSR1, &sa, NULL);

	printf("RIGHT DOUBLE CLICK to test imprecise computation\n");

	ret = setjmp(env);
	if(ret == 0)
	{
		while(1)	
		{
			for(i=0;i<j;i++)
				sum += i;
			flag = flag + 1;
			j+=2;
			sleep(1);
		}
	}
	else
	{
		printf("Total computation time for sum = %d is %d secs\n", sum, flag);
	}
	
return 0;

}