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
	int	n_id;

	if (id % 2 != 0)
		f->is_avaliable[id] = '0';
	else
		f->is_avaliable[id] = '1';
	pthread_mutex_init(&f->fork[id], NULL);
	f->philos[id].condition = TAKE;
	f->philos[id].id = id;
	n_id = f->philos[id].id + 1;
	f->philos[id].n_id = n_id % f->n_philos;
	f->philos[id].fork = (struct s_fork *)f;
	f->philos[id].t_start = get_time();
	f->philos[id].t_now = f->philos[id].t_start;
	f->philos[id].last_eat = f->philos[id].t_start;
}

void	init_mutex(t_fork *f)
{
	static int	i;

	f->philos = (t_philo *)malloc(sizeof(*(f->philos)) * f->n_philos);
	pthread_mutex_init(&f->print, NULL);
	f->fork = malloc(sizeof(pthread_mutex_t) * f->n_philos);
	f->is_avaliable = malloc(sizeof(char) * f->n_philos);
	pthread_mutex_init(&f->l_eat, NULL);
	pthread_mutex_init(&f->d_eat, NULL);
	pthread_mutex_init(&f->is_dead, NULL);
	pthread_mutex_init(&f->nid, NULL);
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
	free(f->philos);
	free(f->is_avaliable);
	free(f->fork);
}

void	init_struct(t_fork *f)
{
	static int	id = -1;

	init_mutex(f);
	while (++id < f->n_philos)
		init_philo(id, f);
	if (f->n_philos % 2 != 0 && f->n_philos != 1)
		f->is_avaliable[f->n_philos - 1] = '0';
	id = -1;
	while (++id < f->n_philos)
	{
		if (pthread_create(&f->philos[id].thread_id,
				NULL, mythreadfun, &f->philos[id]) != 0)
		{
			printf("\nERROR\n");
			return ;
		}
	}
	detroy(f);
}
