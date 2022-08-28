/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:41 by jvelasco          #+#    #+#             */
/*   Updated: 2022/08/28 16:41:43 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	watch_exit(t_fork *f)
{
	static int		x;
	unsigned long	t_now;
	unsigned long	t_time;

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

int	check_dead(t_fork *f)
{
	int	x;

	pthread_mutex_lock(&f->is_dead);
	x = f->died;
	pthread_mutex_unlock(&f->is_dead);
	return (x);
}
