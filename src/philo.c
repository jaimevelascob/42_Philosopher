#include "../inc/philo.h"

int		check_dead(t_fork *f)
{
	int x;

	pthread_mutex_lock(&f->is_dead);
	x = f->died;	
	pthread_mutex_unlock(&f->is_dead);
	return x;
}

int	print(t_fork *f, char *act, int id)
{
	pthread_mutex_lock(&f->is_dead);
	pthread_mutex_lock(&f->print);
	f->philos[id].t_now = get_time();
	f->philos[id].t_time = f->philos[id].t_now - f->philos[id].t_start;
	if (act == "is eating")
	{
		f->philos[id].have_eaten++;
		f->philos[id].last_eat = f->philos[id].t_now;
	}
	if (!f->died) 
		printf("%ldms %d %s\n", f->philos[id].t_time, f->philos[id].id, act);
	pthread_mutex_unlock(&f->print);
	pthread_mutex_unlock(&f->is_dead);
	return (0);
}

int	change_values(t_fork *fork, int h, int z)
{
	print(fork, "is eating", h);
	sleeping(fork,fork->time_eat, h);
	fork->philos[h].can_print = 1;
	fork->philos[z].can_print = 1;
	pthread_mutex_unlock(&fork->philos[h].mutex);
	pthread_mutex_unlock(&fork->philos[z % fork->n_philos].mutex);
	print(fork, "is sleaping", h);
	sleeping(fork,fork->time_sleep, h);
	print(fork, "is thinking", h);
	usleep(100);
}

void	take_fork(t_fork *fork, int id_fork, int id_print)
{
	fork->philos[id_fork].can_print = 0;
	print(fork, "has taken a fork", id_print);
}

void	*mythreadfun(void *vargp)
{
	t_fork	*fork;
	int		h;
	int		z;

	fork = (t_fork *)vargp;
	h = fork->id_fork;
	z = h + 1;
	while (!check_dead(fork)) 
	{
		pthread_mutex_lock(&fork->philos[h].mutex);
		if (fork->philos[h].can_print) 
			take_fork(fork, h, h);
		pthread_mutex_lock(&fork->philos[z % fork->n_philos].mutex);
		if (fork->philos[z % fork->n_philos].can_print) 
			take_fork(fork,z % fork->n_philos, h);
		if (!fork->philos[h].can_print && !fork->philos[z % fork->n_philos].can_print)
			change_values(fork, h, z);
	}
	return (0);
}

void	watch_exit(t_fork *f)
{
	static int	x;
	while (!check_dead(f))
	{
		f->philos[x].t_now = get_time();
		f->philos[x].t_time = f->philos[x].t_now - f->philos[x].t_start;
		if (f->philos[x].have_eaten == f->n_eat)
			join_and_destroy(f, x, '0');
		if ((f->philos[x].t_now - f->philos[x].last_eat) >= f->time_die)
			join_and_destroy(f, x, '1');
		x = (x + 1) % f->n_philos;
	}
}

void	join_and_destroy(t_fork *f, int x, char trigger_dead)
{

	static int	id;
	pthread_mutex_lock(&f->is_dead);
	f->died = 1;
	pthread_mutex_unlock(&f->is_dead);
	if (trigger_dead == '1')
	{
		pthread_mutex_lock(&f->print);
		printf("%ldms %d %s\n", f->philos[x].t_time, x + 1, "is dead");
		pthread_mutex_unlock(&f->print);
	}
	while (id < f->n_philos)
	{
		pthread_join(f->philos[id].thread_id, NULL);
		pthread_mutex_destroy(&f->philos[id].mutex);
		id++;
	}
}
int	main(int argc, char **argv)
{
	t_fork	fork;

	pthread_mutex_init(&fork.print, NULL);
	if (ft_check_arg(&fork, argv, argc) == 0)
		return (0);
	init_struct(&fork);
	watch_exit(&fork);
	return (0);
}
