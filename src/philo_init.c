/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:48 by jvelasco          #+#    #+#             */
/*   Updated: 2022/09/02 16:31:48 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_check_arg(t_fork *fork, char **argv, int argc)
{
	if (argc < 5)
		return (0);
	fork->n_philos = ft_atoi(argv[1]);
	fork->time_die = ft_atoi(argv[2]);
	fork->time_eat = ft_atoi(argv[3]);
	fork->time_sleep = ft_atoi(argv[4]);
	fork->n_eaten = 0;
	if (argc > 5)
		fork->n_eat = ft_atoi(argv[5]);
	else
		fork->n_eat = -1;
	if (fork->n_philos == 0
		|| fork->time_die == 0
		|| fork->time_eat == 0
		|| fork->time_sleep == 0
		|| fork->n_eat == 0)
		return (0);
	fork->died = 0;
	return (1);
}

void	init_philo(int id, t_fork *f)
{
	pthread_mutex_init(&f->philos[id].f_left, NULL);
	f->philos[id].t_start = get_time();
	f->philos[id].t_now = 0;
	f->philos[id].have_eaten = 0;
	f->philos[id].action = 0;
	f->philos[id].last_eat = f->philos[id].t_start;
	f->philos[id].n_fork = 0;
	f->philos[id].is_avaliable = '1';
}

void	init_mutex(t_fork *f)
{
	f->philos = (t_philo *)malloc(sizeof(*(f->philos)) * f->n_philos);
	pthread_mutex_init(&f->print, NULL);
	pthread_mutex_init(&f->l_eat, NULL);
	pthread_mutex_init(&f->d_eat, NULL);
	pthread_mutex_init(&f->is_dead, NULL);
	f->id = 0;
}

void	detroy(t_fork *f)
{
	static int	id;

	while (id < f->n_philos)
	{
		pthread_join(f->philos[id].thread_id, NULL);
		id++;
	}
	pthread_mutex_destroy(&f->d_eat);
	pthread_mutex_destroy(&f->l_eat);
	pthread_mutex_destroy(&f->print);
	pthread_mutex_destroy(&f->is_dead);
	free(f->philos);
}

void	init_struct(t_fork *f)
{
	static int	id = -1;

	init_mutex(f);
	while (++id < f->n_philos)
		init_philo(id, f);
	pthread_create(&f->t_dead, NULL, watch_exit, f);
	id = -1;
	while (++id < f->n_philos)
	{
		f->id_fork = id;
		f->philos[id].id = id + 1;
		f->philos[id].condition = 1;
		pthread_create(&f->philos[id].thread_id, NULL, mythreadfun, f);
		usleep(1);
	}
	detroy(f);
}
