#include "threadpool.h"
# include <pthread.h>

void		bsem_init(t_bsem *bsem, int value)
{
	if (value < 0 || value > 1)
	{
		printf("bsem_init(): Binary semaphore only takes 0 or 1\n");
		exit(1);
	}
	pthread_mutex_init(&bsem->mutex, NULL);
	pthread_cond_init(&bsem->cond, NULL);
	bsem->value = value;
}

void		bsem_signal(t_bsem *bsem)
{
	pthread_mutex_lock(&bsem->mutex);
	bsem->value = 1;
	pthread_cond_signal(&bsem->cond);
	pthread_mutex_unlock(&bsem->mutex);
}

void		bsem_broadcast(t_bsem *bsem)
{
	pthread_mutex_lock(&bsem->mutex);
	bsem->value = 1;
	pthread_cond_broadcast(&bsem->cond);
	pthread_mutex_unlock(&bsem->mutex);
}


void		bsem_wait(t_bsem *bsem)
{
	pthread_mutex_lock(&bsem->mutex);
	while (!bsem->value)
		pthread_cond_wait(&bsem->cond, &bsem->mutex);
	bsem->value = 0;
	pthread_mutex_unlock(&bsem->mutex);
}
