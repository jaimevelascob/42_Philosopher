/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:41 by jvelasco          #+#    #+#             */
/*   Updated: 2022/09/02 16:32:54 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*watch_exit(void *vargp)
{
	t_fork *f;
	static int		x;
	unsigned long	t_now;
	unsigned long	t_time;
	unsigned long	l_eat;
	long			n_eat;

	f = (t_fork *)vargp;

	while (1)
	{
		pthread_mutex_lock(&f->l_eat);
		l_eat = f->philos[x].last_eat;
		pthread_mutex_unlock(&f->l_eat);
		pthread_mutex_lock(&f->d_eat);
		n_eat = f->n_eaten;
		pthread_mutex_unlock(&f->d_eat);
		t_now = get_time();
		t_time = t_now - l_eat;
		if (n_eat == f->n_eat * f->n_philos)
		{
			join_and_destroy(f, x, '0', t_time);
			break ;
		}
		if (t_time >= f->time_die)
		{
			join_and_destroy(f, x, '1', t_time);
			break ;
		}
		x = (x + 1) % f->n_philos;
	}
}

void	join_and_destroy(t_fork *f, int x, char trigger_dead,
		unsigned long time)
{
	static int	id;

	pthread_mutex_lock(&f->is_dead);
	f->died = 1;
	pthread_mutex_unlock(&f->is_dead);
	if (trigger_dead == '1')
	{
		pthread_mutex_lock(&f->print);
		printf("\033[1;31m%ldms %d %s\n", time, x + 1, "is dead");
		pthread_mutex_unlock(&f->print);
	}
	/* while (id < f->n_philos) */
	/* { */
		/* printf("AAAs %d\n", id); */
		/* pthread_join(f->philos[id].thread_id, NULL); */
		/* id++; */
	/* } */
	/* pthread_mutex_destroy(&f->d_eat); */
	/* pthread_mutex_destroy(&f->l_eat); */
	/* pthread_mutex_destroy(&f->print); */
	/* pthread_mutex_destroy(&f->is_dead); */
	/* free(f->philos); */
}

int	check_dead(t_fork *f, int h)
{
	int	x;

	pthread_mutex_lock(&f->is_dead);
	x = f->died;
	/* if (f->n_eaten == f->n_eat * f->n_philos) */
	/* 	x = 1; */
	pthread_mutex_unlock(&f->is_dead);
	return (x);
}
