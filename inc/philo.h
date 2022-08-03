#ifndef PHILO_H
# define PHILO_H
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
	int				can_print;
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
}	t_fork;

void	get_time(t_fork *fork, int num);
void	init_struct(t_fork *f);
long	ft_atoi(const char *str);
int		ft_check_arg(t_fork *fork, char **argv, int argc);
int		print(t_fork *f, char *act, int id);
int		change_values(t_fork *fork, int h, int z);
void	*mythreadfun(void *vargp);
void	watch_exit(t_fork *f, int x);

#endif
