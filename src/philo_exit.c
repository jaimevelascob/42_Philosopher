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
	t_fork			*f;
	unsigned long	last_eat;
	long			n_p_eat;
	unsigned long	t_time;

	f = (t_fork *)vargp;
	while (1)
	{
		last_eat = f->philos[f->id].last_eat;
		n_p_eat = f->n_eaten;
		t_time = get_time() - last_eat;
		if (n_p_eat == f->n_eat * f->n_philos)
		{
			join_and_destroy(f, f->id, '0');
			break ;
		}
		if (t_time >= f->time_die)
		{
			join_and_destroy(f, f->id, '1');
			break ;
		}
		f->id = (f->id + 1) % f->n_philos;
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
		printf(PHILO_DIED, get_time() - f->philos[x].t_start, x + 1);
		pthread_mutex_unlock(&f->print);
	}
}

int	check_dead(t_fork *f, int h)
{
	int	x;

	pthread_mutex_lock(&f->is_dead);
	x = f->died;
	pthread_mutex_unlock(&f->is_dead);
	return (x);
}
