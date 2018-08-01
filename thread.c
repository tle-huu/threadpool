#include "threadpool.h"

void		thread_init(t_thpool *thpool, int i)
{
	if (!thpool || !thpool->threads)
	{
		printf("thread_init(): No thpool or no threads array\n");
		exit(1);
	}
	if (!(thpool->threads[i] = (t_thread *)malloc(sizeof(t_thread))))
	{
		printf("thread_init(): Could not allocate thread number %d\n", i);
		exit(1);
	}
	thpool->threads[i]->id = i;
	thpool->threads[i]->thpool = thpool;
	pthread_create(&(thpool->threads[i]->pthread), NULL, NULL, NULL);
	pthread_detach((thpool->threads[i]->pthread));
}
