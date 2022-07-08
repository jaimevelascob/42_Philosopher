#include "../inc/philo.h"
#include <sys/time.h>
#include <time.h>
/* PASOS */
/* implementar tiempo */
/* implementar argumentos y muerte */
pthread_mutex_t mutex[10];

typedef struct philo{
	int	id;
	double	time_ref;
	int time_now;
	int	time_dif;
} t_philo;

void print(int philo, char *act)
{
	printf("timestamp_in_ms %d %s\n", philo, act);
	/* printf("%d : is sleaping\n", philo); */
	/* printf("%d : is thinking\n", philo); */
}

void *myThreadFun(void *vargp)
{
	t_philo	*philo;

	philo = (t_philo *)vargp;
	printf("%f\n", philo->time_ref/1000);
	int h = philo->id;
	if (h % 2 == 0)
		sleep(1);
	while (1)
	{
		pthread_mutex_lock(&mutex[h]);
		print(h, "has taken a fork");
		pthread_mutex_lock(&mutex[h+1 % 10]);
		print(h, "has taken a fork");
		print(h, "is eating");
		sleep(1);
		pthread_mutex_unlock(&mutex[h+1 % 10]);
		pthread_mutex_unlock(&mutex[h]);
		print(h, "is sleaping");
		sleep(1);
		print(h, "is thinking");
		sleep(1);
	}
	free(vargp);
    return NULL;
}

int main()
{
	t_philo *philo;
	philo = (t_philo *)malloc(sizeof(t_philo) * 10);
	struct timeval start;
	int x = 0;
    pthread_t thread_id[10];
	gettimeofday(&start, NULL);
	philo->time_ref = start.tv_sec;
	while (x++ < 10)
		pthread_mutex_init(&mutex[x], NULL);
	x = 0;
	while (x++ < 10)
	{
		int *a = malloc(sizeof(int));
		*a = x;
		philo->id = *a; 
		pthread_create(&thread_id[x], NULL, myThreadFun, philo);
	}
	x = 0;
	while (x++ < 10)
	{
    	pthread_join(thread_id[x], NULL);
		printf("finish exec %d\n", x);
	}
    printf("After Thread\n");
    exit(0);
}
