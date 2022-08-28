#include "../inc/philo.h"

void	eat(t_fork *f, int id, int id_next)
{
	print(f, "is eating", id);
	f->philos[id].have_eaten++;
	f->philos[id].last_eat = f->philos[id].t_now;
	sleeping(f, f->time_eat, id);
	f->philos[id].have_fork = '0';
	f->philos[id].condition = 2;
	f->philos[id].action = 4;
	f->philos[id_next % f->n_philos].condition = 1;
}

void	take(t_fork *fork, int id_fork, int id_print)
{
	if (fork->philos[id_fork].have_fork == '0')
	{
		fork->philos[id_fork].have_fork = '1';
		print(fork, "has taken a fork", id_fork);
	}
	if (fork->philos[id_print % fork->n_philos].condition == SHARE)
	{
		print(fork, "has taken a fork", id_fork);
		fork->philos[id_fork].action = EAT;
	}
}

void	kip(t_fork *fork, int id)
{
	print(fork, "is sleaping", id);
	sleeping(fork, fork->time_sleep, id);
	fork->philos[id].action = THINK;
}

void	think(t_fork *fork, int id)
{
	print(fork, "is thinking", id);
	fork->philos[id].action = 0;
}
