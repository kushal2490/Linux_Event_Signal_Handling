#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
	
volatile sig_atomic_t sigusr1_flag = 0;
sem_t sem;
pthread_t signalthread_id;

static inline unsigned long long rdtsc()
{
        unsigned long long t;
        __asm__ __volatile__ ("rdtsc" : "=A" (t) );
        return t;
}

void signal_handler(int signo)
{
	sigusr1_flag = 1;
	printf("Signal Handled @ Time = %llu\n",rdtsc());
	return;
}

void* signal_gen_thread(void *arg)
{
	printf("Signal generated @ TIME = %llu\n", rdtsc());
	kill(0, SIGUSR1);
	pthread_exit(0);
}

int main()
{
	struct sigaction sa;
	sigset_t mask;
	struct timespec sleeptime = {0}, sleepleft;
	time_t finish;
	struct sched_param param, setparam;
	int policy;
	int ret;
	int select;

	memset(&sa, 0, sizeof(sa));
	sigfillset(&mask);
	sa.sa_handler = &signal_handler;
	sa.sa_mask = mask;

	sigaction(SIGUSR1, &sa, NULL);

	while(1)
	{
#if 1
		printf("Select a method to demonstrate Signal Handling in Linux\n \
		1) Thread is Running\n \
		2) Thread is Runnable (Running Thread is having higher priority)\n \
		3) Thread is blocked by a semaphore or a File I/O\n \
		4) Thread is delayed (due to nanosleep call)\n\n Method = ");
		scanf("%d", &select);

		//TASK 1 "The Thread is Running"
		if(select == 1)
		{
#endif
#if 1
		printf("\nStarting Task1 \n");
		pthread_create(&signalthread_id, NULL, &signal_gen_thread, NULL);
		while(1)
		{
			if(sigusr1_flag == 1)
			{
				printf("Signal Handling done...\n");
				//sigusr1_flag = 0;
				break;
			}
		}
		printf("Completed Task1\n\n");
		pthread_join(signalthread_id, NULL);
#endif
#if 1
		}


		//TASK 2 "The Thread is runnable (running thread has higher priority)"
		if(select == 2)
		{
#endif
#if 1
		printf("\nStarting Task2 \n");
		ret = pthread_getschedparam(pthread_self(), &policy, &param);
		if(ret != 0)
		{
			printf("Error in getting sched param\n");
		}
		else
		{
			printf("Current thread Priority = %d\n", param.sched_priority);
		}
		setparam.sched_priority = sched_get_priority_max(SCHED_RR);

		ret = pthread_setschedparam(pthread_self(), SCHED_RR, &setparam);
		if(ret != 0)
		{
			printf("Error in setting Priority\n");
		}
		else
		{
			printf("Current Priority set to = %d\n", setparam.sched_priority);
		}
		pthread_create(&signalthread_id, NULL, &signal_gen_thread, NULL);
		while(1)
		{
			if(sigusr1_flag == 1)
			{
				printf("Signal Handling done...\n");
				sigusr1_flag = 0;
				break;
			}
		}
		printf("Completed Task2\n\n");
		pthread_join(signalthread_id, NULL);
#endif
#if 1
		}
		//TASK 3 "The Thread is blocked by semaphore or a file IO"
		if(select == 3)
		{
#endif
#if 1
		printf("\nStarting Task3 \n");
		sem_init(&sem, 0, 1);
		
		pthread_create(&signalthread_id, NULL, &signal_gen_thread, NULL);
		sem_wait(&sem);
		finish = 3 + time(NULL);
		while(time(NULL) < finish)
		{
			ret = sem_wait(&sem);
			if(ret < 0)
			{
				printf("Error in semaphore @ time = %llu\n", rdtsc());
			}
			sem_post(&sem);
		}
		printf("Completed Task3\n\n");
		pthread_join(signalthread_id, NULL);
		//sigusr1_flag = 0;
		sem_destroy(&sem);
#endif
#if 1
		}
		//TASK 4 "The Thread is delayed using nanosleep"
		if(select == 4)
		{
#endif
#if 1
		printf("\nStarting Task4 \n");
		pthread_create(&signalthread_id, NULL, &signal_gen_thread, NULL);

		sleeptime.tv_nsec = 0;
		sleeptime.tv_sec = 5;
		ret = nanosleep(&sleeptime, &sleepleft);
		if(ret<0)
		{
			printf("Error in nanosleep @ Time = %llu\n", rdtsc());
			printf("Time remaining = %u\n", (unsigned int)sleepleft.tv_sec);

		}
		printf("Completed Task4\n\n");

		pthread_join(signalthread_id, NULL);
#endif		
		}

	}
return 0;
}
