#ifndef THREADPOOL_H
# define THREADPOOL_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct		s_bsem
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					value;
}					t_bsem;

typedef struct		s_job
{
	void		(*function)(void *arg);
	void		*arg;
	struct s_job		*next_job;
}					t_job;

typedef struct		s_thread
{
	int							id;
	pthread_t					pthread;
	struct s_thpool				*thpool;
}					t_thread;

typedef struct		s_jobqueue
{
	pthread_mutex_t			rwmutex;
	t_bsem					job_available;
	t_job					*head;
	t_job					*tail;
	int						len;
}					t_jobqueue;

typedef struct		s_thpool
{
	t_thread			**threads;
	volatile int		num_idle;
	volatile int		num_working;
	t_jobqueue			jobqueue;
	pthread_mutex_t		num_lock;
	t_bsem				idle_bsem;

}					t_thpool;


/*
**		BINARY SEMAPHORE
*/
void			bsem_init(t_bsem *bsem, int value);
void			bsem_signal(t_bsem *bsem);
void			bsem_broadcast(t_bsem *bsem);
void			bsem_wait(t_bsem *bsem);
/*
**		THREADPOOL
*/

/*
**		JOBQUEUE
*/

int				jobqueue_init(t_jobqueue *jobqueue);
int				jobqueue_push(t_jobqueue *jobqueue, t_job *job);
t_job			*jobqueue_pop(t_jobqueue *jobqueue);
t_job			*job_create(void (*function)(void *arg), void *arg);

/*
**		THREAD
*/
void			thread_init(t_thpool *thpool, int i);



#endif
