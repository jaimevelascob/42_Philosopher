#include "../inc/philo.h"
#include <sys/time.h>
#include <time.h>
/* TO do */
/* dedatch vs create */

struct fork;
typedef struct philo
{
	pthread_t thread_id;
	int	id;
	int can_print;
	double	time_ref;
	unsigned long time;
	int	time_dif;
	int	eat;
	pthread_mutex_t mutex;
	struct timeval start;
	struct timeval now;
	struct t_fork *fork;
}	t_philo;

typedef struct fork 
{
	t_philo *philos;
	int	id_fork;
}	t_fork;


void print(t_philo philo, char *act)
{
	gettimeofday(&philo.now, NULL);

	unsigned long now_mill = (philo.now.tv_sec) * 1000 + (philo.now.tv_usec) / 1000;
	unsigned long total_time = now_mill - philo.time;
	printf("%ldms %d %s\n", total_time, philo.id, act);
}

void *myThreadFun(void *vargp)
{
	t_fork	*fork;

	fork = (t_fork *)vargp;
	int h = fork->id_fork;
	int z = h+1;
	if (h % 2 != 0)
		usleep(10000);
	fork->philos[h].eat = 0; 
	while (1)
	{
		/* printf("%d %d h-: %d h+1: %d\n",h,z%11 , fork->philos[h].can_print, fork->philos[z%11].can_print); */
		if (fork->philos[h].can_print && fork->philos[z%11].can_print)
		{
			fork->philos[h].eat += 2; 
			fork->philos[h].can_print = 0;
			fork->philos[z].can_print = 0;
			pthread_mutex_lock(&fork->philos[h].mutex);
			print(fork->philos[h], "has taken a fork");
			pthread_mutex_lock(&fork->philos[z%11].mutex);
			print(fork->philos[h], "has taken a fork");
		}
		if (fork->philos[h].eat == 2)
		{
			print(fork->philos[h], "is eating");
			usleep(10000);
			print(fork->philos[h], "is sleaping");
			fork->philos[h].eat = 0; 
			pthread_mutex_unlock(&fork->philos[h].mutex);
			pthread_mutex_unlock(&fork->philos[z%11].mutex);
			fork->philos[h].can_print = 1;
			fork->philos[z].can_print = 1;
			usleep(10000);
			print(fork->philos[h], "is thinking");
			usleep(10000);
		}
	}
    return NULL;
}
/*
 * FIX
 * añadir argumentos
 * añadir muerte
 * */
int main()
{
	t_fork fork;
	fork.philos = (t_philo*)malloc(sizeof(*(fork.philos)) * 10);
	int x = -1;
	while (++x < 11)
	{
		pthread_mutex_init(&fork.philos[x].mutex, NULL);
		gettimeofday(&fork.philos[x].start, NULL);
		fork.philos[x].time = (fork.philos[x].start.tv_sec * 1000) + (fork.philos[x].start.tv_usec / 1000);
	}
	x = -1;
	while (++x < 11)
	{
		fork.id_fork = x;
		fork.philos[x].id = x+1; 
		fork.philos[x].can_print = 1; 
		pthread_create(&fork.philos[x].thread_id, NULL, myThreadFun, &fork);
		usleep(1);
	}
	x = -1;
	while (x++ < 10)
	{
    	pthread_join(fork.philos->thread_id, NULL);
		printf("finish exec %d\n", x);
	}
	
    printf("After Thread\n");
    exit(0);
}
