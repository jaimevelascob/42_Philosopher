/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:41:53 by jvelasco          #+#    #+#             */
/*   Updated: 2022/08/30 18:40:25 by jvelasco         ###   ########.fr       */
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
	unsigned long	second;

	now = get_time();
	while (1)
	{
		second = get_time();
		if (get_time() - now >= time || fork->died)
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
