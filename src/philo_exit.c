/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:41 by jvelasco          #+#    #+#             */
/*   Updated: 2023/03/13 14:38:44 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
