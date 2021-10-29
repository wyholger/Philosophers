#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

unsigned long		get_time_in_ms(void)
{
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void take_fork(t_data *data, int fork, int phil)
{
	unsigned long int time;
	
	pthread_mutex_lock(&data->forks[fork]);
	time = (get_time_in_ms() - data->t_start);
	printf("%6ld %d has taken a fork\n", time, phil);
}

void death(t_data *data, t_ph *ph)
{
	unsigned long int time;
	
	time = (get_time_in_ms() - data->t_start);
	if (data->death_flag == 0)
	{
		data->death_flag = 1;
		printf("%6ld %d died\n", time, ph->item);
	}
	
}

void *check_death(void *arg)
{
	t_ph	*ph;
	
	ph = (t_ph*)arg;
	pthread_mutex_init(&ph->mtx_local_death, NULL);
	pthread_mutex_init(&ph->mtx_eat_time, NULL);
	pthread_mutex_lock(&ph->mtx_eat_time);
//	printf("%ld <= %d ", get_time_in_ms() - ph->t_last_eat, ph->data->t_die);
	while (get_time_in_ms() - ph->t_last_eat <= (unsigned long int)ph->data->t_die)
	{
		pthread_mutex_unlock(&ph->mtx_eat_time);
		usleep(100);
		pthread_mutex_lock(&ph->mtx_eat_time);
	}
	ph->data->death = 1;
	ph->death = 1;
	return (NULL);
}

void	eating(t_data *data, t_ph *ph)
{
	unsigned long int time;
	
	time = (get_time_in_ms() - data->t_start);
	if (data->must_eat != -1 && ph->eat_of_time <= data->must_eat)
	{
		pthread_mutex_lock(&ph->mtx_eat_time);
		ph->eat_of_time++;
		printf("%6ld %d is eating. Eat of time %d\n", time, ph->item, ph->eat_of_time);
		ph->t_last_eat = get_time_in_ms();
		pthread_mutex_unlock(&ph->mtx_eat_time);
		usleep(data->t_eat * 1000);
	}
	pthread_mutex_unlock(&data->forks[ph->r_fork]);
	pthread_mutex_unlock(&data->forks[ph->l_fork]);
}

void sleeping(t_data *data, t_ph *ph)
{
	unsigned long int time;
	
	time = (get_time_in_ms() - data->t_start);
	printf("%6ld %d is sleeping\n", time, ph->item);
	usleep(data->t_sleep * 1000);
}

void thinking(t_data *data, t_ph *ph)
{
	unsigned long int time;
	
	time = (get_time_in_ms() - data->t_start);
	printf("%6ld %d is thinking\n", time, ph->item);
}

void *philo(void *arg)
{
	t_ph		*ph;
	pthread_t	t_death;
	
	ph = (t_ph*)arg;
//	printf("Number philo is %d, l_fork %d, r_fork %d, l_philo %d, r_philo %d\n", ph->item, ph->l_fork, ph->r_fork, ph->l_phil, ph->r_phil);
	ph->t_last_eat = ph->data->t_start;
	pthread_create(&t_death, NULL, check_death, &ph[0]);
	while (1)
	{
		if (ph->data->death == 1)
			break ;
		if (ph->data->priority[ph->item] > ph->data->priority[ph->l_phil] && ph->data->priority[ph->item] > ph->data->priority[ph->r_phil])
		{
			take_fork(ph->data, ph->l_fork, ph->item);
			take_fork(ph->data, ph->r_fork, ph->item);
			eating(ph->data, ph);
			ph->data->priority[ph->item] -= 2;
			if (ph->data->death == 1)
				break ;
			sleeping(ph->data, ph);
			if (ph->data->death == 1)
				break ;
			thinking(ph->data, ph);
		}
		if (ph->data->death == 1)
			break ;
		usleep(100);
	}
	pthread_join(t_death, NULL);
	if (ph->death == 1)
		death(ph->data, ph);
	return (NULL);
}

void	check_argc(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error, number arguments error\n");
		exit (0);
	}
}

int	is_positive_digit(int character)
{
	if ((character >= '0' && character <= '9') || character == '+')
	{
		return (1);
	}
	return (0);
}

void	check_valid_argv(char *argv[], int argc)
{
	int	i;
	int	j;
	
	i = 1;
	j = 0;
	while (i < argc)
	{
		while (argv[i][j])
		{
			if (is_positive_digit((int)argv[i][j]))
				j++;
			else
			{
				printf("Error. Not valid argument.\n");
				exit (0);
			}
		}
		j = 0;
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int					i;
	unsigned long int	rezult;
	int					single;
	
	i = 0;
	rezult = 0;
	single = 1;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n')
		   || (str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			single = single * -1;
	}
	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		rezult = (rezult * 10) + (str[i] - '0');
		i++;
		if (rezult > 9223372036854775807 && single > 0)
			return (-1);
		if (rezult > 9223372036854775807 && single < 0)
			return (0);
	}
	return ((int)rezult * single);
}

void	parsing(t_data *data, int argc, char *argv[])
{
	check_argc(argc);
	check_valid_argv(argv, argc);
	data->num_phil = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
}

void	init_null_data(t_data *data)
{
	int i;
	
	i = 0;
	data->priority = malloc(sizeof(int) * data->num_phil);
	while (i < data->num_phil)
	{
		if (i % 2 == 0)
			data->priority[i] = 0;
		else
			data->priority[i] = 1;
		i++;
	}
//	if (data->num_phil % 2 != 0)
//		data->priority[data->num_phil - 1] = 3;
	data->death = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->death_flag = 0;
}

void destroy_forks(t_data *data, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i--;
	}
}

void init_forks(t_data *data)
{
	int	i;
	
	i = 0;
	data->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->num_phil);
	if (!data->forks)
	{
		printf("Error. Mutex - forks init error.\n");
		exit(0);
	}
	while (i < data->num_phil)
	{
		if (!pthread_mutex_init(&data->forks[i], NULL))
			i++;
		else
		{
			destroy_forks(data, i);
			printf("Error. Mutex - forks init error.\n");
			exit(0);
		}
	}
}

t_ph *init_philo_sruct(t_data *data, t_ph *ph)
{
	int i;
	
	i = 0;
	ph = (t_ph*)malloc(sizeof(t_ph) * data->num_phil);
	if (!ph)
	{
		printf("Error. Pthread - philo init error.\n");
		destroy_forks(data, data->num_phil);
		exit(0);
	}
	while (i < data->num_phil)
	{
		ph[i].eat_of_time = 0;
		ph[i].death = 0;
		ph[i].data = data;
		ph[i].item = i;
		ph[i].l_fork = i;
		if (i + 1 != data->num_phil)
		{
			ph[i].r_fork = i + 1;
			ph[i].r_phil = i + 1;
		}
		else
		{
			ph[i].r_fork = 0;
			ph[i].r_phil = 0;
		}
		if (i == 0)
			ph[i].l_phil = data->num_phil - 1;
		else
			ph[i].l_phil = i - 1;
		
		i++;
	}
	data->ph = ph;
	return (ph);
}

void init_philo(t_data *data)
{
	int	i;
	t_ph *ph;
	
	i = 0;
	ph = NULL;
	ph = init_philo_sruct(data, ph);
	data->philo = (pthread_t*)malloc(sizeof(pthread_t) * data->num_phil);
	if (!data->philo)
	{
		printf("Error. Pthread - philo init error.\n");
		destroy_forks(data, data->num_phil);
		exit(0);
	}
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i])) //need else
			i++;
		else
			return ;
	}
}

void join_thread(t_data *data)
{
	int i;
	
	i = 0;
	while (i < data->num_phil)
	{
		pthread_join(data->philo[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->num_phil)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

void	end_cleaner(t_data *data)
{
	free(data->forks);
	free(data->philo);
	free(data->ph);
	free(data->priority);
}

void init_time_start(t_data *data)
{
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	data->t_start = (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void start(t_data *data)
{
	init_forks(data);
	init_time_start(data);
	init_philo(data);
	join_thread(data);
	end_cleaner(data);
}

int main(int argc, char *argv[])
{
	t_data	data;
//	struct timeval		tv1;
//	struct timeval		tv2;
//	struct timezone		tz;
	init_null_data(&data);
	parsing(&data, argc, argv);
	start(&data);
//	gettimeofday(&tv1, &tz);
//	tv2.tv_sec = tv1.tv_sec;
//	tv2.tv_usec = tv1.tv_usec;
//	printf("%ld\n", tv1.tv_usec - tv2.tv_usec);
//	gettimeofday(&tv1, &tz);
//	printf("%ld\n", tv1.tv_usec - tv2.tv_usec);
//	printf("%ld\n", tv1.tv_usec);
//	pthread_t t1;
//	pthread_create(&t1, NULL, philo, NULL);
//	pthread_join(t1, NULL);
	return 0;
}
