#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"


void task1(){
	printf("Thread #%u  working on task1\n", (int)pthread_self());

	printf("Thread #%u  finished with task1\n", (int)pthread_self());
}


void task2(){
	printf("Thread #%u working on task2\n", (int)pthread_self());

	printf("Thread #%u finished with task2\n", (int)pthread_self());
}


int main(){

	puts("Making threadpool with 4 threads");
	t_thpool *thpool = thpool_init(6);

	puts("Adding 40 tasks to threadpool");
	int i;
	for (i=0; i<20; i++){
		thpool_add_work(thpool, (void*)task1, NULL);
		thpool_add_work(thpool, (void*)task2, NULL);
	};
	puts("Killing threadpool");
	sleep(1);
	thpool_destroy(thpool);

	return 0;
}
