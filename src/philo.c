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
	unsigned long time_start;
	unsigned long last_eat;
	int	time_dif;
	pthread_mutex_t mutex;
	struct timeval start;
	struct timeval now;
	struct t_fork *fork;
}	t_philo;

typedef struct fork 
{
	t_philo *philos;
	int	id_fork;
	long	n_philos;
	long	n_eat;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	pthread_mutex_t print;
}	t_fork;


void print(t_fork fork, char *act, int num)
{
	pthread_mutex_lock(&fork.print);
	gettimeofday(&fork.philos[num].now, NULL);

	unsigned long now_mill = (fork.philos[num].now.tv_sec) * 1000 + (fork.philos[num].now.tv_usec) / 1000;
	if (act == "is eating")
		fork.philos[num].last_eat = now_mill;
	/* printf("%ld \n", (now_mill - fork.philos[num].last_eat)); */
	if ((now_mill - fork.philos[num].last_eat) > fork.time_die)
		printf("%d %s\n", fork.philos[num].id, "is dead");
	unsigned long total_time = now_mill - fork.philos[num].time_start;
	printf("%ldms %d %s\n", total_time, fork.philos[num].id, act);
	pthread_mutex_unlock(&fork.print);
}

void *myThreadFun(void *vargp)
{
	t_fork	*fork;

	fork = (t_fork *)vargp;
	int h = fork->id_fork;
	int z = h+1;
	if (h % 2 != 0)
		usleep(fork->time_eat);
	while (1)
	{
		/* printf("%d %d h-: %d h+1: %d\n",h,z%11 , fork->philos[h].can_print, fork->philos[z%11].can_print); */
		pthread_mutex_lock(&fork->philos[h].mutex);
		pthread_mutex_lock(&fork->philos[z % fork->n_philos].mutex);
		if (fork->philos[h].can_print && fork->philos[z % fork->n_philos].can_print)
		{
			fork->philos[h].can_print = 0;
			fork->philos[z].can_print = 0;
			print(*fork, "has taken a fork", h);
			print(*fork, "has taken a fork", h);
			print(*fork, "is eating", h);
			usleep(fork->time_eat);
			fork->philos[h].can_print = 1;
			fork->philos[z].can_print = 1;
			pthread_mutex_unlock(&fork->philos[h].mutex);
			pthread_mutex_unlock(&fork->philos[z % fork->n_philos].mutex);
			print(*fork, "is sleaping", h);
			usleep(fork->time_sleep);
			print(*fork, "is thinking", h);
		}
	}
    return NULL;
}
/*
 * FIX
 * muerte funciona, falta hacer acabar el programa cuando muera alguien
 * */

long	ft_atoi(const char *str)
{
	int		i;
	int		oper;
	long	res;

	i = 0;
	oper = 1;
	if (str[i] == '-')
	{
		i++;
		oper *= -1;
	}
	res = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			res = (res * 10) + (str[i++] - '0');
		else
			return (0);
	}
	return (res * oper);
}

int	ft_check_arg(t_fork *fork, char **argv, int argc)
{
	if (argc < 5)
		return 0;
	fork->n_philos = ft_atoi(argv[1]);
	fork->time_die = ft_atoi(argv[2]);
	fork->time_eat = ft_atoi(argv[3]) * 1000;
	fork->time_sleep = ft_atoi(argv[4]) * 1000;
	if (argc > 5)
		fork->n_eat = ft_atoi(argv[5]);

	if (fork->n_philos == 0 ||
		fork->time_die == 0 ||
		fork->time_eat == 0 ||
		fork->time_sleep == 0 ||
		fork->n_eat == 0)
		return (0);
	return (1);
}

void	init_struct(t_fork *fork)
{
	int	x;

	x = -1;
	fork->philos = (t_philo*)malloc(sizeof(*(fork->philos)) * fork->n_philos);
	while (++x < fork->n_philos)
	{
		pthread_mutex_init(&fork->philos[x].mutex, NULL);
		gettimeofday(&fork->philos[x].start, NULL);
		fork->philos[x].time_start = (fork->philos[x].start.tv_sec * 1000) + (fork->philos[x].start.tv_usec / 1000);
		fork->philos[x].last_eat = fork->philos[x].time_start;
	}
	x = -1;
	while (++x < fork->n_philos)
	{
		fork->id_fork = x;
		fork->philos[x].id = x+1;
		fork->philos[x].can_print = 1;
		pthread_create(&fork->philos[x].thread_id, NULL, myThreadFun, fork);
		usleep(1);
	}
}

int main(int argc, char **argv)
{
	t_fork fork;
	int	x = 0;
	/* init_parametres */
	pthread_mutex_init(&fork.print, NULL);
	if	(ft_check_arg(&fork, argv, argc) == 0)
		return	(0);
	init_struct(&fork);
	x = -1;
	while (x++ < fork.n_philos)
	{
    	pthread_join(fork.philos->thread_id, NULL);
		printf("finish exec %d\n", x);
	}
    exit(0);
}
