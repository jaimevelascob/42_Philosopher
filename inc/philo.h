/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvelasco <jvelasco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 16:42:25 by jvelasco          #+#    #+#             */
/*   Updated: 2022/09/02 16:29:41 by jvelasco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define TAKE 1
# define SHARE 2
# define EAT 3
# define SLEEP 4
# define THINK 5
# define REPEAT 50
# define LEFT_FORK_TAKEN  "\033[1;32m \
[%ldms] \tphilo %d has taken left fork\n\033[0;39m"
# define RIGHT_FORK_TAKEN  "\033[1;32m \
[%ldms] \tphilo %d has taken right fork\n\033[0;39m"
# define PHILO_EATS  "\033[0;33m \
[%ldms] \tphilo %d is eating \n\033[0;39m"
# define PHILO_SLEEPS  "\033[0;34m \
[%ldms] \tphilo %d is sleaping\n\033[0;39m"
# define PHILO_THINKS  "\033[0;36m \
[%ldms] \tphilo %d is thinking\n\033[0;39m"
# define PHILO_DIED  "\033[0;31m \
[%ldms] \tphilo %d is dead\n\033[0;39m"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h> 
# include <pthread.h>
# include <sys/time.h>
# include <time.h>
typedef struct s_philo s_philo;
typedef struct s_fork s_fork;
struct s_philo 
{
	pthread_t		thread_id;
	int				id;
	int				n_id;
	int				condition;
	unsigned long	last_eat;
	unsigned long	t_now;
	unsigned long	t_start;
	s_fork			*fork;
};

struct s_fork
{
	s_philo			*philos;
	long			n_philos;
	long			n_eat;
	long			n_eaten;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				id;
	int				id_fork;
	int				died;
	char			*is_avaliable;
	pthread_mutex_t	*fork;
	pthread_t		t_dead;
	pthread_mutex_t	print;
	pthread_mutex_t	l_eat;
	pthread_mutex_t	d_eat;
	pthread_mutex_t	is_dead;
};

/* philo.c */
int					print(s_philo f, char *act, int id);
void				*mythreadfun(void *vargp);
/* philo_utils.c */
void				sleeping(s_fork *fork, unsigned long time, int id);
unsigned long		get_time(void);
long				ft_atoi(const char *str);
/* philo_init.c */
int					ft_check_arg(s_fork *fork, char **argv, int argc);
void				init_struct(s_fork *f);
void				init_philo(int id, s_fork *f);
void				detroy(s_fork *f);
void				init_mutex(s_fork *f);
/* philo_exit.c */
void				*watch_exit(void *vargp);
void				join_and_destroy(s_fork *f, int x, char trigger_dead);
int					check_dead(s_fork *f, int h);
/* philo_actions.c */
void				eat(s_fork *fork, int h, int z);
void				take(s_fork *p, int id, int n_id);
void				kip(s_philo *p, long time);
void				think(s_philo *p, long time);
#endif
