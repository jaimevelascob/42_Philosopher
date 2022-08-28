/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:58 by jvelasco          #+#    #+#             */
/*   Updated: 2022/08/28 16:42:00 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	print(t_fork *f, char *act, int id)
{
	pthread_mutex_lock(&f->is_dead);
	pthread_mutex_lock(&f->print);
	f->philos[id].t_now = get_time();
	f->philos[id].t_time = f->philos[id].t_now - f->philos[id].t_start;
	if (!f->died)
		printf("%ldms %d %s\n", f->philos[id].t_time, f->philos[id].id, act);
	pthread_mutex_unlock(&f->print);
	pthread_mutex_unlock(&f->is_dead);
	return (0);
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
		if (fork->philos[h].condition == TAKE)
			take(fork, h, z);
		if (fork->philos[h].action == EAT)
			eat(fork, h, z);
		if (fork->philos[h].action == SLEEP)
			kip(fork, h);
		if (fork->philos[h].action == THINK)
			think(fork, h);
		usleep(REPEAT);
	}
	return (0);
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
