#include "../inc/philo.h"

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

void	init_philo(int id, t_fork *f)
{
	pthread_mutex_init(&f->philos[id].mutex, NULL);
	f->philos[id].t_start = get_time();
	f->philos[id].last_eat = f->philos[id].t_start;
	f->philos[id].t_now = 0;
	f->philos[id].have_eaten = 0;
	f->philos[id].action = 0;
	f->philos[id].last_eat = f->philos[id].t_start;
	f->philos[id].have_fork = '0';
}

void	init_struct(t_fork *f)
{
	int	id;

	id = -1;
	f->philos = (t_philo *)malloc(sizeof(*(f->philos)) * f->n_philos);
	pthread_mutex_init(&f->is_dead, NULL);
	pthread_mutex_init(&f->mutex, NULL);
	while (++id < f->n_philos)
		init_philo(id, f);
	if (f->n_philos % 2 != 0)
		f->philos[f->n_philos - 1].condition = SHARE;
	id = -1;
	while (++id < f->n_philos)
	{
		f->id_fork = id;
		f->philos[id].id = id + 1;
		f->philos[id].condition = 1;
		pthread_create(&f->philos[id].thread_id, NULL, mythreadfun, f);
		usleep(1);
	}
}
