/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:58 by jvelasco          #+#    #+#             */
/*   Updated: 2023/03/17 15:05:23 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	print(t_philo f, char *act, int id)
{
	unsigned long	t_time;

	pthread_mutex_lock(&f.fork->print);
	t_time = f.t_now - f.t_start;
	pthread_mutex_lock(&f.fork->l_eat);
	if (!check_dead(f.fork, 1) && f.fork->n_eat)
		printf(act, t_time, id + 1, act);
	pthread_mutex_unlock(&f.fork->l_eat);
	pthread_mutex_unlock(&f.fork->print);
	return (0);
}

int	check_philo(t_philo *p)
{
	if (p->t_now - p->last_eat > p->fork->time_die)
	{
		join_and_destroy(p->fork, p->id, '1');
		return (0);
	}
	pthread_mutex_lock(&p->fork->l_eat);
	if (p->fork->n_eat != -1 && p->fork->n_eat <= 0)
	{
		join_and_destroy(p->fork, p->id, '0');
		pthread_mutex_unlock(&p->fork->l_eat);
		return (0);
	}
	pthread_mutex_unlock(&p->fork->l_eat);
	return (1);
}

void	*mythreadfun(void *vargp)
{
	t_philo	*p;

	p = vargp;
	while (!check_dead(p->fork, 1) && check_philo(p))
	{
		p->t_now = get_time();
		if (check_philo(p) && !check_dead(p->fork, 1))
		{
			if (p->condition == TAKE)
				take(p->fork, p->id, p->n_id);
			else if (p->condition == SLEEP)
				kip(p, p->fork->time_eat);
			else if (p->condition == THINK)
				think(p, p->fork->time_sleep);
		}
		usleep(50);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_fork	fork;

	if (ft_check_arg(&fork, argv, argc) == 0)
	{
		printf("to make it work:\n");
		printf("1. Number of philos\n");
		printf("2. Time die\n3. Time eat\n");
		printf("4. Time sleep\n5. (Optional) ");
		printf("Number of eats\n");
		return (0);
	}
	init_struct(&fork);
	return (0);
}
