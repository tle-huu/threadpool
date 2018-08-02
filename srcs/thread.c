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
	pthread_create(&(thpool->threads[i]->pthread), NULL, (void *)thread_work, (void *)thpool->threads[i]);
	pthread_detach((thpool->threads[i]->pthread));
}

void		thread_destroy(t_thread *thread)
{
	// free(thread);
}

void		*thread_work(t_thread *thread)
{
	t_thpool		*pool;
	t_job			*job;
	int				test = 0;

	if (!thread)
	{
		printf("thread_work(): thread NULL pointer\n");
		return (NULL);
	}
	pool = thread->thpool;
	pthread_mutex_lock(&(pool->num_lock));
	pool->num_alive++;
	pthread_mutex_unlock(&(pool->num_lock));
	while (pool->alive)
	{
		printf("Thread [%d] -> [%d] looping\n", thread->id, (int)pthread_self());
		if (!(pool->jobqueue.len))
		{
			printf("Thread [%d] -> [%d] waiting 1 (%d)\n", thread->id, (int)pthread_self(), thread->thpool->jobqueue.len);
			test = 0;
			bsem_wait(&(pool->jobqueue.job_available));
			test = 1;
			printf("Thread [%d] -> [%d] waiting 2\n", thread->id, (int)pthread_self());
		}
		// printf("before if\n");
		if (pool->alive)
		{
			if (!test)
				pthread_mutex_lock(&(pool->num_lock));
			pool->num_working++;
			pthread_mutex_unlock(&(pool->num_lock));
			if ((job = jobqueue_pop(&(pool->jobqueue))))
			{
				job->function(job->arg);
				free(job);
			}
		}
		pool->num_working--;
	}
	pthread_mutex_lock(&(pool->num_lock));
	pool->num_alive--;
	pthread_mutex_unlock(&(pool->num_lock));
	return (NULL);
}
