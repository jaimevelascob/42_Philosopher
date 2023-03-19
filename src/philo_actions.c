/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:08 by jvelasco          #+#    #+#             */
/*   Updated: 2023/03/17 14:47:32 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	prepare_eat(t_fork *p, int id, int n_id)
{
	p->philos[id].last_eat = p->philos[id].t_now;
	print(p->philos[id], RIGHT_FORK_TAKEN, id);
	eat(p, id, n_id);
	p->philos[id].condition = SLEEP;
	p->is_avaliable[n_id] = '1';
	p->philos[id].n_fork = 0;
	p->philos[id].t_now += p->time_eat;
	print(p->philos[id], PHILO_SLEEPS, id);
}

void	eat(t_fork *f, int id, int id_next)
{
	print(f->philos[id], PHILO_EATS, id);
	sleeping(f, f->time_eat, id);
	pthread_mutex_lock(&f->l_eat);
	if (f->n_eat != -1)
		f->n_eat--;
	pthread_mutex_unlock(&f->l_eat);
}

void	take(t_fork *p, int id, int n_id)
{
	pthread_mutex_lock(&p->fork[id]);
	if (p->is_avaliable[id] == '1' && check_philo(p->philos[id]))
	{
		p->philos[id].t_now = get_time();
		print(p->philos[id], LEFT_FORK_TAKEN, id);
		p->is_avaliable[id] = '0';
		p->philos[id].n_fork++;
	}
	pthread_mutex_unlock(&p->fork[id]);
	if (p->n_philos != 1)
	{
		pthread_mutex_lock(&p->fork[n_id]);
		if (p->is_avaliable[n_id] == '0'
			&& p->is_avaliable[id] == '0' && p->philos[id].n_fork
			&& check_philo(p->philos[id]))
			prepare_eat(p, id, n_id);
		pthread_mutex_unlock(&p->fork[n_id]);
	}
}

void	kip(t_philo *p, long time)
{
	if (p->t_now - p->last_eat < p->fork->time_die)
	{
		sleeping(p->fork, time, p->id);
		p->condition = THINK;
	}
	else
		join_and_destroy(p->fork, p->id, '1');
}

void	think(t_philo *p, long time)
{
	if (p->t_now - p->last_eat < p->fork->time_die)
	{
		print(*p, PHILO_THINKS, p->id);
		usleep(REPEAT);
		p->condition = TAKE;
	}
	else
		join_and_destroy(p->fork, p->id, '1');
}
