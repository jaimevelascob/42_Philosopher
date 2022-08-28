/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:42:25 by jvelasco          #+#    #+#             */
/*   Updated: 2022/08/28 16:42:27 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define TAKE 1
# define SHARE 2
# define EAT 3
# define SLEEP 4
# define THINK 5
# define REPEAT 100
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h> 
# include <pthread.h>
# include <sys/time.h>
# include <time.h>

struct	s_fork;
typedef struct philo
{
	pthread_t		thread_id;
	int				id;
	char			have_fork;
	int				condition;
	int				action;
	unsigned long	last_eat;
	unsigned long	t_now;
	unsigned long	t_time;
	unsigned long	t_start;
	int				have_eaten;
	pthread_mutex_t	mutex;
	struct timeval	start;
	struct timeval	now;
	struct t_fork	*fork;
}	t_philo;

typedef struct s_fork
{
	t_philo			*philos;
	int				id_fork;
	long			n_philos;
	long			n_eat;
	long			time_die;
	int				died;
	long			time_eat;
	long			time_sleep;
	pthread_mutex_t	print;
	pthread_mutex_t	mutex;
	pthread_mutex_t	is_dead;
}	t_fork;

/* philo.c */
int					print(t_fork *f, char *act, int id);
void				*mythreadfun(void *vargp);
/* philo_utils.c */
void				sleeping(t_fork *fork, unsigned long time, int id);
unsigned long		get_time(void);
long				ft_atoi(const char *str);
/* philo_init.c */
int					ft_check_arg(t_fork *fork, char **argv, int argc);
void				init_struct(t_fork *f);
void				init_philo(int id, t_fork *f);
/* philo_exit.c */
void				watch_exit(t_fork *f);
void				join_and_destroy(t_fork *f, int x, char trigger_dead,
						unsigned long time);
int					check_dead(t_fork *f);
/* philo_actions.c */
void				eat(t_fork *fork, int h, int z);
void				take(t_fork *fork, int id_fork, int id_print);
void				kip(t_fork *fork, int id);
void				think(t_fork *fork, int id);
#endif
