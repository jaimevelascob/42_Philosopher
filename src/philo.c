/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:58 by jvelasco          #+#    #+#             */
/*   Updated: 2022/09/02 16:28:10 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	print(s_philo f, char *act, int id)
{
	unsigned long t_time;

	t_time = f.t_now - f.t_start;
	pthread_mutex_lock(&f.fork->print);
	if (!f.fork->died)
		printf(act, t_time, id + 1, act);
	pthread_mutex_unlock(&f.fork->print);
	return (0);
}

void	*mythreadfun(void *vargp)
{
	s_philo	*p;
	int	time_delay;

	p = vargp;
	if (p->id % 2 != 0)
		usleep(5);
	while (!p->fork->died)
	{
		if (p->condition == TAKE)
			take(p->fork, p->id, p->n_id);
		else if (p->condition == SLEEP)
			kip(p, p->fork->time_eat);
		else if (p->condition == THINK)
			think(p, p->fork->time_sleep);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	s_fork	fork;

	if (ft_check_arg(&fork, argv, argc) == 0)
		return (0);
	init_struct(&fork);
	return (0);
}
