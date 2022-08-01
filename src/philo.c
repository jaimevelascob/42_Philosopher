#include "../inc/philo.h"

void	get_time(t_fork *fork, int num)
{
	gettimeofday(&fork->philos[num].now, NULL);
	fork->philos[num].t_now = (fork->philos[num].now.tv_sec) * 1000 + (fork->philos[num].now.tv_usec) / 1000;
	fork->philos[num].t_time = fork->philos[num].t_now - fork->philos[num].t_start;
}

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
		return (0);
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
	fork->died = 0;
	return (1);
}

void	init_struct(t_fork *f)
{
	int	x;

	x = -1;
	f->philos = (t_philo*)malloc(sizeof(*(f->philos)) * f->n_philos);
	while (++x < f->n_philos)
	{
		pthread_mutex_init(&f->philos[x].mutex, NULL);
		gettimeofday(&f->philos[x].start, NULL);
		f->philos[x].t_start = (f->philos[x].start.tv_sec * 1000) + (f->philos[x].start.tv_usec / 1000);
		f->philos[x].last_eat = f->philos[x].t_start;
		f->philos[x].t_now = 0;
		f->philos[x].have_eaten = 0;
		f->philos[x].last_eat = f->philos[x].t_start;
	}
	x = -1;
	while (++x < f->n_philos)
	{
		f->id_fork = x;
		f->philos[x].id = x+1;
		f->philos[x].can_print = 1;
		pthread_create(&f->philos[x].thread_id, NULL, myThreadFun, f);
		usleep(1);
	}
}

int	print(t_fork *f, char *act, int id)
{
	pthread_mutex_lock(&f->print);
	get_time(f, id);
	if (act == "is eating")
	{
		f->philos[id].have_eaten++;
		f->philos[id].last_eat = f->philos[id].t_now;
	}
	printf("%ldms %d %s\n", f->philos[id].t_time, f->philos[id].id, act);
	pthread_mutex_unlock(&f->print);
	return (0);
}

int	change_values(t_fork *fork, int h, int z)
{
	fork->philos[h].can_print = 0;
	fork->philos[z].can_print = 0;
	print(fork, "has taken a fork", h);
	print(fork, "has taken a fork", h);
	print(fork, "is eating", h);
	usleep(fork->time_eat);
	fork->philos[h].can_print = 1;
	fork->philos[z].can_print = 1;
	pthread_mutex_unlock(&fork->philos[h].mutex);
	pthread_mutex_unlock(&fork->philos[z % fork->n_philos].mutex);
	print(fork, "is sleaping", h);
	usleep(fork->time_sleep);
	print(fork, "is thinking", h);
}

void	*myThreadFun(void *vargp)
{
	t_fork	*fork;

	fork = (t_fork *)vargp;
	int h = fork->id_fork;
	int z = h + 1;
	if (h % 2 != 0)
		usleep(fork->time_eat);
	while (1)
	{
		pthread_mutex_lock(&fork->philos[h].mutex);
		pthread_mutex_lock(&fork->philos[z % fork->n_philos].mutex);
		change_values(fork, h, z);
	}
    return (0);
}

void	watch_exit(t_fork *f, int x)
{
	while(1)
	{
		get_time(f, x);
		if (f->philos[x].have_eaten == f->n_eat)
		{
			pthread_mutex_lock(&f->print);
			while (++x < f->n_philos)
				pthread_mutex_destroy(&f->philos[x].mutex);
			free(f->philos);
			exit(0);
		}
		if ((f->philos[x].t_now - f->philos[x].last_eat) > f->time_die)
		{
			pthread_mutex_lock(&f->print);
			printf("%d %s\n", f->philos[x].id,  "is dead");
			while (++x < f->n_philos)
				pthread_mutex_destroy(&f->philos[x].mutex);
			free(f->philos);
			exit(0);
		}
		x = (x + 1) % f->n_philos;
	}
}

int main(int argc, char **argv)
{
	t_fork		fork;
	static	int x;
	pthread_mutex_init(&fork.print, NULL);
	if	(ft_check_arg(&fork, argv, argc) == 0)
		return (0);
	init_struct(&fork);
	watch_exit(&fork, x);
	pthread_mutex_unlock(&fork.print);
	return (0);
}
