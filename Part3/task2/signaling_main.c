#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include "lib_signaling.h"

pthread_t signal_thread, thread_id[5];

/* signal handlers for particular thread functions */
void* thread_signal_handler1(int signo)
{
	printf("Signal handled by thread 1\n");
	return NULL;
}
void* thread_signal_handler2(int signo)
{
	printf("Signal handled by thread 2\n");
	return NULL;
}
void* thread_signal_handler3(int signo)
{
	printf("Signal handled by thread 3\n");
	return NULL;
}
void* thread_signal_handler4(int signo)
{
	printf("Signal handled by thread 4\n");
	return NULL;
}
void* thread_signal_handler5(int signo)
{
	printf("Signal handled by thread 5\n");
	return NULL;
}

/* Thread function handlers which register a particular thread with a signal handler */
void* thread_func1(void *arg)
{
	allocate_thread(pthread_self(), thread_signal_handler1);
	return NULL;
}
void* thread_func2(void *arg)
{
	allocate_thread(pthread_self(), thread_signal_handler2);
	return NULL;
}
void* thread_func3(void *arg)
{
	allocate_thread(pthread_self(), thread_signal_handler3);
	return NULL;
}
void* thread_func4(void *arg)
{
	//Not allocating Signal to be handled by this thread
	return NULL;
}
void* thread_func5(void *arg)
{
	allocate_thread(pthread_self(), thread_signal_handler5);
	return NULL;
}

int main()
{

//Create multiple threads to handle signal by blocking the signal for a few
	pthread_create(&thread_id[1], NULL, thread_func1, NULL);
	pthread_create(&thread_id[2], NULL, thread_func2, NULL);
	pthread_create(&thread_id[3], NULL, thread_func3, NULL);
	pthread_create(&thread_id[4], NULL, thread_func4, NULL);
	pthread_create(&thread_id[5], NULL, thread_func5, NULL);
	pthread_create(&signal_thread, NULL, signal_thread_func, NULL);

	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[2], NULL);
	pthread_join(thread_id[3], NULL);
	pthread_join(thread_id[4], NULL);
	pthread_join(thread_id[5], NULL);
	pthread_join(signal_thread, NULL);

return 0;
}
