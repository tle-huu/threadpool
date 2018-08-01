#include "threadpool.h"

t_thpool	*thpool_init(int num_threads)
{
	t_thpool		*thpool;


	if (num_threads <= 0)
	{
		printf("Thpool_init(): Number of thread zero\n");
		num_threads = 0;
	}

	/* New thread pool */
	if (!(thpool = (t_thpool *)malloc(sizeof(t_thpool))))
	{
		printf("thpool(): thpool malloc failed\n");
		return (NULL);
	}
	thpool->alive = 1;
	thpool->num_alive = 0;
	thpool->num_working = 0;
	thpool->num_threads = num_threads;
	pthread_mutex_init(&thpool->num_lock, NULL);

	/* Initialise the job queue */
	if (jobqueue_init(&(thpool->jobqueue)) == -1)
	{
		printf("thpool(): jobqueue allocation failed\n");
		free(thpool);
		return (NULL);
	}

	/* Initialise the threads in pool */
	if (!(thpool->threads = (t_thread **)calloc((1 + num_threads), sizeof(t_thread *))))
	{
		printf("thpool(): thpool threads malloc failed\n");
		// jobqueue_destroy(&thpool->jobqueue);
		free(thpool);
		return (NULL);
	}

	/*	Initialise threads */
	for (int i = 0; i < num_threads; i++)
	{
		thread_init(thpool, i);
		printf("Created threads %d\n", i);
	}
	/* Wait for threads to initialize */
	while (thpool->num_alive != num_threads) {printf("debug init wait\n");}
	// printf("Thpool_init(): OK\n");
	return (thpool);
}

int			thpool_destroy(t_thpool *thpool)
{
	if (!(thpool))
	{
		printf("thpool_destroy(): thpool NULL\n");
		return (-1);
	}
	printf("Len : %d\n", thpool->jobqueue.len);
	thpool->alive = 0;
    //
	/* Wait for thread to die */
	sleep(1);
	for (int i = 0; thpool->threads[i]; i++)
	{
		printf("i : %d\n", i);
		thread_destroy(thpool->threads[i]);
	}
	printf("Len : %d\n", thpool->jobqueue.len);
	jobqueue_destroy(&thpool->jobqueue);
	// free(thpool->threads);
	// free(thpool);
	return (1);
}

int			thpool_add_work(t_thpool *thpool, void (*function)(void *arg), void *arg)
{
	t_job		*newjob;

	// printf("before job create\n");
	if (!(newjob = job_create(function, arg)))
		return (-1);
	// printf("after job create\n");
	return (jobqueue_push(&(thpool->jobqueue), newjob));
}
