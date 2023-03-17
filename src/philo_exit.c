/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:41 by jvelasco          #+#    #+#             */
/*   Updated: 2023/03/17 14:04:52 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	join_and_destroy(t_fork *f, int x, char trigger_dead)
{
	static int	id;

	/* f->philos[x].t_now = get_time(); */
	/* print(f->philos[id], PHILO_DIED, id); */
	/* pthread_mutex_lock(&f->is_dead); */
	/* f->died = 1; */
	/* pthread_mutex_unlock(&f->is_dead); */

	pthread_mutex_lock(&f->print);
	pthread_mutex_lock(&f->d_eat);
	/* f->philos[x].t_now = get_time(); */
	if (trigger_dead == '1' && !check_dead(f, 1))
	{
		printf(PHILO_DIED, 	f->philos[x].t_now - f->philos[x].t_start, x + 1);
	}
	pthread_mutex_lock(&f->is_dead);
	f->died = 1;
	pthread_mutex_unlock(&f->is_dead);
	pthread_mutex_unlock(&f->d_eat);
	pthread_mutex_unlock(&f->print);
}

int	check_dead(t_fork *f, int h)
{
	int	x;

	pthread_mutex_lock(&f->is_dead);
	x = f->died;
	pthread_mutex_unlock(&f->is_dead);
	return (x);
}
