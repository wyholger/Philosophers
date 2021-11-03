#include "philo.h"

unsigned long		get_time_in_ms(void)
{
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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
	data->death = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->waiter = NULL;
	data->death_flag = 0;
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

void destroy_forks(t_data *data, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->can_eat[i]);
		i--;
	}
}

void init_forks(t_data *data)
{
	int	i;
	
	i = 0;
	data->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->num_phil);
	data->can_eat = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->num_phil);
	data->poel = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->num_phil);
	data->need_eat = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->num_phil);
	if (!data->forks)
	{
		printf("Error. Mutex - forks init error.\n");
		exit(0);
	}
	while (i < data->num_phil)
	{
		if (!pthread_mutex_init(&data->forks[i], NULL) && !pthread_mutex_init(&data->can_eat[i], NULL) && !pthread_mutex_init(&data->poel[i], NULL) && !pthread_mutex_init(&data->need_eat[i], NULL))
		{
//			pthread_mutex_lock(&data->forks[i]);
			i++;
		}
		else
		{
			destroy_forks(data, i);
			printf("Error. Mutex - forks init error.\n");
			exit(0);
		}
	}
}

void init_time_start(t_data *data)
{
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	data->t_start = (time.tv_sec * 1000 + time.tv_usec / 1000);
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

void take_fork(t_data *data, int fork, int phil)
{
	unsigned long int time;
	
	pthread_mutex_lock(&data->forks[fork]);
	time = (get_time_in_ms() - data->t_start);
	if (data->death != 1)
		printf("%6ld %d has taken a fork #%d\n", time, phil, fork);
//	else
//		pthread_mutex_unlock(&data->forks[fork]);
}

void *check_death(void *arg)
{
	t_ph	*ph;
	
	ph = (t_ph*)arg;
//	pthread_mutex_init(&ph->mtx_local_death, NULL);
	pthread_mutex_init(&ph->mtx_eat_time, NULL);
	pthread_mutex_lock(&ph->mtx_eat_time);
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

int min_fork(t_ph *ph)
{
	if (ph->l_fork < ph->r_fork)
		return (ph->l_fork);
	else
		return (ph->r_fork);
}

int max_fork(t_ph *ph)
{
	if (ph->l_fork > ph->r_fork)
		return (ph->l_fork);
	else
		return (ph->r_fork);
}

void	eating(t_data *data, t_ph *ph)
{
	unsigned long int time;
	
	if ((data->must_eat != -1 && ph->eat_of_time <= data->must_eat && ph->data->death != 1) || (data->must_eat == -1 && ph->data->death != 1))
	{
		pthread_mutex_lock(&ph->mtx_eat_time);
		ph->eat_of_time++;
		time = (get_time_in_ms() - data->t_start);
		printf("%6ld %d is eating. Eat of time %d\n", time, ph->item, ph->eat_of_time);
		ph->t_last_eat = get_time_in_ms();
		pthread_mutex_unlock(&ph->mtx_eat_time);
		usleep(data->t_eat * 1000);
	}
	
	pthread_mutex_unlock(&data->forks[ph->l_fork]);
	pthread_mutex_unlock(&data->forks[ph->r_fork]);
//	pthread_mutex_unlock(&data->forks[max_fork(ph)]);
//	pthread_mutex_unlock(&data->forks[min_fork(ph)]);
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

void *philo(void *arg)
{
	t_ph		*ph;
	pthread_t	t_death;
	
	ph = (t_ph*)arg;
	ph->t_last_eat = ph->data->t_start;
	pthread_create(&t_death, NULL, check_death, &ph[0]);
	usleep(1000);
//	if (ph->item % 2 == 0)
//		pthread_mutex_lock(&ph->data->can_eat[ph->item]);
	while (1)
	{
		if (ph->item == 0 || ph->item == ph->data->num_phil - 1)
		{
			pthread_mutex_unlock(&ph->data->need_eat[ph->item]);
		}
		pthread_mutex_unlock(&ph->data->can_eat[ph->item]);
		pthread_mutex_lock(&ph->data->poel[ph->item]);
		if (ph->data->death == 1)
			break ;
		take_fork(ph->data, ph->l_fork, ph->item);
		if (ph->l_fork == ph->r_fork)
			break ;
		take_fork(ph->data, ph->r_fork, ph->item);
//		take_fork(ph->data, min_fork(ph), ph->item);
//		take_fork(ph->data, max_fork(ph), ph->item);
		eating(ph->data, ph);
		if (ph->item == 0 || ph->item == ph->data->num_phil - 1) {
			pthread_mutex_lock(&ph->data->need_eat[ph->item]);
		}
		pthread_mutex_unlock(&ph->data->poel[ph->item]);
		ph->data->priority[ph->item] -= 2;
		if (ph->data->death == 1)
			break ;
		sleeping(ph->data, ph);
		if (ph->data->death == 1)
			break ;
		thinking(ph->data, ph);
		if (ph->data->death == 1)
			break ;
	}
	pthread_join(t_death, NULL);
	if (ph->death == 1)
		death(ph->data, ph);
	pthread_mutex_unlock(&ph->data->poel[ph->item]);
	pthread_mutex_unlock(&ph->data->need_eat[ph->item]);
	pthread_mutex_unlock(&ph->data->can_eat[ph->item]);
//	printf("END %d\n", ph->item);
	return (NULL);
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
		pthread_mutex_destroy(&data->can_eat[i]);
		pthread_mutex_destroy(&data->poel[i]);
		pthread_mutex_destroy(&data->need_eat[i]);
		i++;
	}
}

void *start_waiter(void *arg)
{
	t_data	*data;
	
	data = arg;
	while (data->death != 1)
	{
//		pthread_mutex_lock(&data->can_eat[1]);//лочим второго
		pthread_mutex_lock(&data->need_eat[data->num_phil -1]);//лочим последнего
		if (data->death == 1)
			pthread_mutex_unlock(&data->need_eat[data->num_phil -1]);
//		if (data->death != 1)
		pthread_mutex_unlock(&data->poel[0]);//ждем пока поест первый
//		if (data->death != 1)
		pthread_mutex_unlock(&data->need_eat[data->num_phil -1]);//разлочиваем последнего
//		pthread_mutex_unlock(&data->can_eat[1]);//разлочим второго
	}
	
	return (NULL);
}

void *end_waiter(void *arg)
{
	t_data	*data;
	
	data = arg;
	while (data->death != 1)
	{
//		pthread_mutex_lock(&data->can_eat[data->num_phil - 2]);//лочим предпосленего философа
		
		pthread_mutex_lock(&data->need_eat[0]);//лочим 0 философа
		if (data->death == 1)
			pthread_mutex_unlock(&data->need_eat[0]);
//		if (data->death != 1)
		pthread_mutex_unlock(&data->poel[data->num_phil -1]);//ждем пока поест последний философ
//		if (data->death != 1)
		pthread_mutex_unlock(&data->need_eat[0]);//разлочиваем первого
//		pthread_mutex_unlock(&data->can_eat[data->num_phil - 2]);//разлочим предпоследнего философа
	}
	return (NULL);
}

void *waiter(void *arg)
{
	pthread_t	s_waiter;
	pthread_t	e_waiter;
	
	
	t_data	*data;
	int		i;
//	t_ph	*ph;
	
	data = arg;

//	ph = NULL;
//	ph = init_philo_sruct(data, ph);
//	data->philo = (pthread_t*)malloc(sizeof(pthread_t) * data->num_phil);
	i = 1;
	while (i + 1 < data->num_phil)
	{
		pthread_mutex_lock(&data->can_eat[i]);
		i += 2;
	}
	i = 0;
	while (i < data->num_phil)
	{
		pthread_mutex_unlock(&data->poel[i]);
		pthread_mutex_unlock(&data->can_eat[i]);
		i++;
	}
	if (data->num_phil % 2 == 1 && data->num_phil != 1)
	{
		pthread_create(&s_waiter, NULL, start_waiter, data);
		pthread_create(&e_waiter, NULL, end_waiter, data);
		pthread_join(s_waiter, NULL);
		pthread_join(e_waiter, NULL);
	}
//	printf("IIIIIIIIIIIIIIIII\n");
	return (NULL);
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

void	init_waiter(t_data *data, pthread_t	*waiter_thread)
{
	data->waiter = waiter_thread;
	pthread_create(waiter_thread, NULL, waiter, &data[0]);
//	pthread_join(*data->waiter, NULL);
}

void	end_cleaner(t_data *data)
{
	free(data->forks);
	free(data->can_eat);
	free(data->poel);
	free(data->need_eat);
	free(data->philo);
	free(data->ph);
	free(data->priority);
}

void start(t_data *data)
{
	pthread_t	waiter_thread;
	
	init_forks(data);
	init_time_start(data);
	init_waiter(data, &waiter_thread);
//	usleep(1000);
	init_philo(data);
	join_thread(data);
	pthread_join(waiter_thread, NULL);
	end_cleaner(data);
}

int main(int argc, char *argv[])
{
	t_data	data;
	init_null_data(&data);
	parsing(&data, argc, argv);
	start(&data);
	return 0;
}
