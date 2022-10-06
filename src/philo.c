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

int	print(t_fork *f, char *act, int id)
{
	f->philos[id].t_time = f->philos[id].t_now - f->philos[id].t_start;
	pthread_mutex_lock(&f->print);
	if (!check_dead(f, id))
		printf(act, f->philos[id].t_time, f->philos[id].id, act);
	pthread_mutex_unlock(&f->print);
	return (0);
}

void	*mythreadfun(void *vargp)
{
	t_fork	*fork;
	int		id;
	int		id_next;
	int		time_delay;

	time_delay = 0;
	fork = (t_fork *)vargp;
	id = fork->id_fork;
	fork->philos[id].t_start = get_time();
	fork->philos[id].last_eat = fork->philos[id].t_start;
	if (id % 2 != 0)
		time_delay = fork->time_eat;
	fork->philos[id].t_now = fork->philos[id].t_start + time_delay;
	id_next = id + 1;
	while (!check_dead(fork, id))
	{
		if (fork->philos[id].condition == TAKE)
			take(fork, id, id_next);
		if (fork->philos[id].action == SLEEP)
			kip(fork, id);
		if (fork->philos[id].action == THINK)
			think(fork, id);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_fork	fork;

	if (ft_check_arg(&fork, argv, argc) == 0)
		return (0);
	init_struct(&fork);
	return (0);
}
