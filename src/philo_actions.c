/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:08 by jvelasco          #+#    #+#             */
/*   Updated: 2022/09/02 16:31:12 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	eat(t_fork *f, int id, int id_next)
{
	print(f->philos[id], PHILO_EATS, id);
	f->n_eaten++;
	sleeping(f, f->time_eat, id);
	f->philos[id].last_eat = get_time();
}

void	take(t_fork *p, int id, int n_id)
{
	pthread_mutex_lock(&p->fork[id]);
	if (p->is_avaliable[id] == '1')
	{
		p->philos[id].t_now = get_time();
		print(p->philos[id], LEFT_FORK_TAKEN, id);
		p->is_avaliable[id] = '0';
	}
	if (p->n_philos != 1)
	{
		pthread_mutex_lock(&p->fork[n_id]);
		if (p->is_avaliable[n_id] == '1'
			&& p->is_avaliable[id] == '0')
		{
			p->is_avaliable[n_id] = '0';
			p->philos[id].t_now = get_time();
			print(p->philos[id], RIGHT_FORK_TAKEN, id);
			eat(p, id, n_id);
			p->is_avaliable[n_id] = '1';
			p->philos[id].condition = SLEEP;
			p->is_avaliable[id] = '1';
		}
		pthread_mutex_unlock(&p->fork[n_id]);
	}
	pthread_mutex_unlock(&p->fork[id]);
}

void	kip(t_philo *p, long time)
{
	p->t_now += time;
	print(*p, PHILO_SLEEPS, p->id);
	sleeping(p->fork, time, p->id);
	p->condition = THINK;
}

void	think(t_philo *p, long time)
{
	p->t_now += time;
	print(*p, PHILO_THINKS, p->id);
	usleep(REPEAT);
	p->condition = TAKE;
}
