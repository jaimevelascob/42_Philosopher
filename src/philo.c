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
	int		id;
	int		n_id;

	time_delay = 0;
	p = vargp;
	p->t_start = get_time();
	p->last_eat = p->t_start;
	if (p->id % 2 != 0)
		time_delay = 100;
	p->t_now = p->t_start + time_delay;
	while (!p->fork->died)
	{
		take(p->fork, p->id, p->n_id);
		kip(p, p->fork[0].time_eat);
		think(p,p->fork[0].time_sleep);
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
