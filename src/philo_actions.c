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
	print(f, PHILO_EATS, id);
	pthread_mutex_lock(&f->d_eat);
	f->n_eaten++;
	pthread_mutex_unlock(&f->d_eat);
	sleeping(f, f->time_eat, id);
	pthread_mutex_lock(&f->l_eat);
	f->philos[id].last_eat = get_time();
	pthread_mutex_unlock(&f->l_eat);
	f->philos[id].action = 4;
	f->philos[id_next % f->n_philos].is_avaliable = '1';
	f->philos[id].is_avaliable = '1';
}

void	take(t_fork *fork, int id_fork, int id_print)
{
	pthread_mutex_lock(&fork->philos[id_fork].f_left);
	if (fork->philos[id_fork].is_avaliable == '1')
	{
		print(fork, LEFT_FORK_TAKEN, id_fork);
		fork->philos[id_fork].is_avaliable = '0';
	}
	if (fork->n_philos != 1)
	{
		pthread_mutex_lock(&fork->philos[id_print % fork->n_philos].f_left);
		if (fork->philos[id_print % fork->n_philos].is_avaliable == '1'
			&& fork->philos[id_fork].is_avaliable == '0')
		{
			print(fork, RIGHT_FORK_TAKEN, id_fork);
			fork->philos[id_print % fork->n_philos].is_avaliable = '0';
			eat(fork, id_fork, id_print);
		}
		pthread_mutex_unlock(&fork->philos[id_print % fork->n_philos].f_left);
	}
	pthread_mutex_unlock(&fork->philos[id_fork].f_left);
}

void	kip(t_fork *fork, int id)
{
	fork->philos[id].t_now += 100;
	print(fork, PHILO_SLEEPS, id);
	sleeping(fork, fork->time_sleep, id);
	fork->philos[id].action = THINK;
}

void	think(t_fork *fork, int id)
{
	fork->philos[id].t_now += 100;
	print(fork, PHILO_THINKS, id);
	fork->philos[id].action = 0;
	fork->philos[id].condition = 1;
}
