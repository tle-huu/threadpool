#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"


void task1(){
	printf("\033[34;1mThread #%u working on task1 \033[0m\n", (int)pthread_self());
}


void task2(){
	printf("\033[32;1mThread #%u working on task2 \033[0m\n", (int)pthread_self());
	// exit(1);
}


int main(){

	puts("Making threadpool with 4 threads");
	t_thpool *thpool = thpool_init(1000);

	puts("Adding 40 tasks to threadpool");
	int i;
	for (i=0; i<2000; i++){
		thpool_add_work(thpool, (void*)task1, NULL);
		thpool_add_work(thpool, (void*)task2, NULL);
	};
	sleep(1);
	puts("Killing threadpool");
	thpool_destroy(thpool);

	return 0;
}
