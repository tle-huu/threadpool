#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"


void job1(){
	printf("\033[34;1m I am Thread [%d] and I am working on job1 \033[0m\n", (int)pthread_self());
}


void job2(){
	printf("\033[32;1m I am Thread [%d] and I am working on job2 \033[0m\n", (int)pthread_self());
	// sleep(1);
}

void job3(){
	sleep(1);
	printf("\033[33;1m I am Thread [%d] and I am working on job3 \033[0m\n", (int)pthread_self());
	// sleep(1);
}

int main(){

	clock_t start, end;
    double cpu_time_used;

    start = clock();

	printf("Initialising thread pool\n");
	t_thpool *thpool = thpool_init(42);

	printf(" Pushing task to do into the job queue\n");
	int i;
	for (i=0; i< 200; i++){
		thpool_add_work(thpool, (void*)job1, NULL);
		thpool_add_work(thpool, (void*)job2, NULL);
		thpool_add_work(thpool, (void*)job3, NULL);
	};
	sleep(1);

	thpool_destroy(thpool);
	end = clock();
    cpu_time_used = ((double) (end - start));
	printf(" ============= [%f] ===========\n", cpu_time_used / CLOCKS_PER_SEC);
	return 0;
}
