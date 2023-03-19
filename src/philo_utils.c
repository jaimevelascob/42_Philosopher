/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:53 by jvelasco          #+#    #+#             */
/*   Updated: 2023/03/17 14:54:57 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

unsigned long	get_time(void)
{
	unsigned long	time;
	struct timeval	t_now;

	gettimeofday(&t_now, NULL);
	time = t_now.tv_sec * 1000 + t_now.tv_usec / 1000;
	return (time);
}

void	sleeping(t_fork *fork, unsigned long time, int id)
{
	unsigned long	now;
	const int		n_id = fork->philos[id].n_id;

	now = get_time();
	while (1)
	{
		if (get_time() - fork->philos[id].last_eat > fork->time_die)
			join_and_destroy(fork, id, '1');
		if (get_time() - fork->philos[n_id].last_eat > fork->time_die)
			join_and_destroy(fork, n_id, '1');
		if (get_time() - now >= time || check_dead(fork, 1))
			break ;
		usleep(50);
	}
}

long	ft_atoi(const char *str)
{
	int		i;
	int		oper;
	long	res;

	i = 0;
	oper = 1;
	if (str[i] == '-')
	{
		i++;
		oper *= -1;
	}
	res = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			res = (res * 10) + (str[i++] - '0');
		else
			return (0);
	}
	return (res * oper);
}

int	check_numbers(t_fork *fork, char a)
{
	if (fork->n_philos <= 0 || fork->n_philos > 200)
	{
		printf(BAD_NUMBER);
		return (0);
	}
	if (fork->time_die <= 0 || fork->time_eat <= 0
		|| fork->time_sleep <= 0 || (a != 'a' && fork->n_eat <= 0))
	{
		printf(BAD_TIME);
		return (0);
	}
	return (1);
}
