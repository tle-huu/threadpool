#ifndef THREADPOOL_H
# define THREADPOOL_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <strings.h>

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
	volatile int		num_alive;
	volatile int		num_threads;
	volatile int		num_working;
	t_jobqueue			jobqueue;
	pthread_mutex_t		num_lock;
	int					alive;
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
t_thpool	*thpool_init(int num_thread);
int			thpool_add_work(t_thpool *thpool, void (*function)(void *arg), void *arg);
int			thpool_destroy(t_thpool *thpool);

/*
**		THREAD
*/
void			thread_init(t_thpool *thpool, int i);
void			thread_destroy(t_thread *thread);
void			*thread_work(t_thread *thread);



/*
**		JOBQUEUE
*/

int				jobqueue_init(t_jobqueue *jobqueue);
int				jobqueue_push(t_jobqueue *jobqueue, t_job *job);
t_job			*jobqueue_pop(t_jobqueue *jobqueue);
t_job			*job_create(void (*function)(void *arg), void *arg);
void			jobqueue_clear(t_jobqueue *jobqueue);
void			jobqueue_destroy(t_jobqueue *jobqueue);



#endif
