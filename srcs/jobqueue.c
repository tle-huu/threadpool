#include "threadpool.h"

int			jobqueue_init(t_jobqueue *jobqueue)
{
	if (!jobqueue)
		return (-1);
	pthread_mutex_init(&(jobqueue->rwmutex), NULL);
	jobqueue->head = NULL;
	jobqueue->tail = NULL;
	jobqueue->len = 0;
	bsem_init(&(jobqueue->job_available), 0);
	return (1);
}

int			jobqueue_push(t_jobqueue *jobqueue, t_job *job)
{
	// printf("\033[32;1m Start of push \033[0m ");
	pthread_mutex_lock(&(jobqueue->rwmutex));
	// printf("\033[34;1m push lock push\033[0m ");
	job->next_job = NULL;
	if (!jobqueue || !job)
	{
		printf("jobqueue_push(): NULL arguments\n");
		return (-1);
	}
	if (!jobqueue->len)
	{
		jobqueue->head = job;
		jobqueue->tail = job;
	}
	else
	{
		jobqueue->tail->next_job = job;
		jobqueue->tail = job;
	}
	jobqueue->len++;
	bsem_signal(&(jobqueue->job_available));
	// printf("push unlock mutex\n");
	pthread_mutex_unlock(&(jobqueue->rwmutex));
	// printf("end of push\n");
	return (1);
}

t_job		*jobqueue_pop(t_jobqueue *jobqueue)
{
	t_job	*job;

	pthread_mutex_lock(&(jobqueue->rwmutex));
	if (!jobqueue)
	{
		printf("jobqueue_push(): NULL arguments\n");
		return (NULL);
	}
	job = jobqueue->head;
	if (!job)
		return (job);
	if (jobqueue->len == 1)
	{
		jobqueue->head = NULL;
		jobqueue->tail = NULL;
	}
	else if (jobqueue->len)
	{
		jobqueue->head = jobqueue->head->next_job;
		bsem_signal(&(jobqueue->job_available));
	}
	jobqueue->len--;
	pthread_mutex_unlock(&(jobqueue->rwmutex));
	return (job);
}

t_job		*job_create(void (*function)(void *arg), void *arg)
{
	t_job		*job;

	if (!function)
	{
		printf("job_create(): Function pointer NULL\n");
		return (NULL);
	}
	if (!(job = (t_job *)malloc(sizeof(t_job))))
	{
		printf("job_create(): Could not malloc t_job\n");
		return (NULL);
	}
	job->function = function;
	job->arg = arg;
	job->next_job = NULL;
	return (job);
}

void		jobqueue_clear(t_jobqueue *jobqueue)
{
	if (jobqueue)
	{
		while (jobqueue->len)
			free(jobqueue_pop(jobqueue));
		jobqueue->head = NULL;
		jobqueue->tail = NULL;
		/* bset reset ?? */
		jobqueue->len = 0;
	}
}

void		jobqueue_destroy(t_jobqueue *jobqueue)
{
	jobqueue_clear(jobqueue);
}
