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
	fork->philos[h].have_fork = '0';
	fork->philos[h].condition = 2; 
	fork->philos[z % fork->n_philos].condition = 1;
	print(fork, "is sleaping", h);
	sleeping(fork,fork->time_sleep, h);
	print(fork, "is thinking", h);
	usleep(100);
}

void	take_fork(t_fork *fork, int id_fork, int id_print)
{
	if (fork->philos[id_fork].have_fork == '0')
	{
		fork->philos[id_fork].have_fork = '1';
		print(fork, "has taken a fork", id_fork);
	}
	if (fork->philos[id_print % fork->n_philos].condition == SHARE)
	{
		print(fork, "has taken a fork", id_fork);
		fork->philos[id_fork].condition = 3;
	}
}

void	*mythreadfun(void *vargp)
{
	t_fork	*fork;
	int		h;
	int		z;

	fork = (t_fork *)vargp;
	h = fork->id_fork;
	if (h % 2 != 0)
		fork->philos[h].condition = SHARE;
	
	z = h + 1;
	while (!check_dead(fork)) 
	{
		pthread_mutex_lock(&fork->philos[h].mutex);
		if (fork->philos[h].condition == TAKE) 
			take_fork(fork, h, z);
		pthread_mutex_unlock(&fork->philos[h].mutex);
		if (fork->philos[h].condition == EAT)
			change_values(fork, h, z);
	}
	return (0);
}

void	watch_exit(t_fork *f)
{
	static int	x;
	unsigned long t_now;
	unsigned long t_time;
	while (!check_dead(f))
	{
		t_now = get_time();
		t_time = t_now - f->philos[x].t_start;
		if (f->philos[x].have_eaten == f->n_eat)
			join_and_destroy(f, x, '0', t_time);
		if ((t_now - f->philos[x].last_eat) >= f->time_die)
			join_and_destroy(f, x, '1', t_time);
		x = (x + 1) % f->n_philos;
	}
}

void	join_and_destroy(t_fork *f, int x, char trigger_dead, unsigned long time)
{

	static int	id;
	pthread_mutex_lock(&f->is_dead);
	f->died = 1;
	pthread_mutex_unlock(&f->is_dead);
	if (trigger_dead == '1')
	{
		pthread_mutex_lock(&f->print);
		printf("%ldms %d %s\n", time, x + 1, "is dead");
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
