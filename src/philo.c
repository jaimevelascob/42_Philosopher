#include "../inc/philo.h"

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

void	*mythreadfun(void *vargp)
{
	t_fork	*fork;
	int		h;
	int		z;

	fork = (t_fork *)vargp;
	h = fork->id_fork;
	z = h + 1;
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
	while (1)
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
			printf("%ldms %d %s\n", f->philos[x].t_time, x + 1, "is dead");
			while (++x < f->n_philos)
				pthread_mutex_destroy(&f->philos[x].mutex);
			free(f->philos);
			exit(0);
		}
		x = (x + 1) % f->n_philos;
	}
}

int	main(int argc, char **argv)
{
	t_fork		fork;
	static int	x;

	pthread_mutex_init(&fork.print, NULL);
	if (ft_check_arg(&fork, argv, argc) == 0)
		return (0);
	init_struct(&fork);
	watch_exit(&fork, x);
	pthread_mutex_unlock(&fork.print);
	return (0);
}
