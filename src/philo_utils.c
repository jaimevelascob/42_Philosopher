#include "../inc/philo.h"

unsigned long	get_time()
{

	unsigned long time;
	struct timeval	t_now;

	gettimeofday(&t_now, NULL);
	time = t_now.tv_sec * 1000 + t_now.tv_usec / 1000;
	return time;
}

void	sleeping(t_fork *fork, unsigned long time, int id)
{
	unsigned long	now;
	unsigned long	second;

	now = get_time();
	while (1)
	{
		second = get_time();
		if (get_time() - now >= time)
			break;
		usleep(50);
	}
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
	fork->time_eat = ft_atoi(argv[3]);
	fork->time_sleep = ft_atoi(argv[4]);
	if (argc > 5)
		fork->n_eat = ft_atoi(argv[5]);
	if (fork->n_philos == 0
		|| fork->time_die == 0
		|| fork->time_eat == 0
		|| fork->time_sleep == 0
		|| fork->n_eat == 0)
		return (0);
	fork->died = 0;
	return (1);
}

void	init_struct(t_fork *f)
{
	int	x;

	x = -1;
	f->philos = (t_philo *)malloc(sizeof(*(f->philos)) * f->n_philos);
	pthread_mutex_init(&f->is_dead, NULL);
	while (++x < f->n_philos)
	{
		pthread_mutex_init(&f->philos[x].mutex, NULL);
		gettimeofday(&f->philos[x].start, NULL);
		f->philos[x].t_start = (f->philos[x].start.tv_sec * 1000)
			+ (f->philos[x].start.tv_usec / 1000);
		f->philos[x].last_eat = f->philos[x].t_start;
		f->philos[x].t_now = 0;
		f->philos[x].have_eaten = 0;
		f->philos[x].last_eat = f->philos[x].t_start;
	}
	x = -1;
	while (++x < f->n_philos)
	{
		f->id_fork = x;
		f->philos[x].id = x + 1;
		f->philos[x].can_print = 1;
		pthread_create(&f->philos[x].thread_id, NULL, mythreadfun, f);
		usleep(1);
	}
}
