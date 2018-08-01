#include "threadpool.h"

t_thpool	*thpool_init(int num_thread)
{
	t_thpool		*thpool;

	if (num_thread < 0)
		num_thread = 0;

	/* New thread pool */
	if (!(thpool = (t_thpool *)malloc(sizeof(t_thpool))))
	{
		printf("thpool(): thpool malloc failed\n");
		return (NULL);
	}
	thpool->num_idle = 0;
	thpool->num_working = 0;
	pthread_mutex_init(&thpool->num_lock, NULL);

	/* Initialise the job queue */
	if (jobqueue_init(&(thpool->jobqueue)) == -1)
	{
		printf("thpool(): jobqueue allocation failed\n");
		free(thpool);
		return (NULL);
	}

	/* Initialise the threads in pool */
	if (!(thpool->threads = (t_thread **)malloc(sizeof(t_thread *) * (1 + num_thread))))
	{
		printf("thpool(): thpool threads malloc failed\n");
		// jobqueue_destroy(&thpool->jobqueue);
		free(thpool);
		return (NULL);
	}
	bsem_init(&thpool->idle_bsem, 1);

	/*	Initialise threads */
	for (int i = 0; i < num_thread; i++)
		thread_init(thpool, i);
	return (thpool);
}
