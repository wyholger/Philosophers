#include "philo.h"

unsigned long		get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void init_time_start(t_data *data)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	data->t_start = (time.tv_sec * 1000 + time.tv_usec / 1000);
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
	data->death = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->death_flag = 0;
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
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

t_ph *init_philo_sruct(t_data *data, t_ph *ph)
{
	int i;

	i = 0;
	ph = (t_ph*)malloc(sizeof(t_ph) * data->num_phil);
	if (!ph)
	{
		printf("Error. Pthread - philo init error.\n"); //add destructor
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

int write_msg(unsigned long int time, t_ph *ph, char *msg)
{
	if (get_time_in_ms() - ph->t_last_eat <= (unsigned long int)ph->data->t_die && ph->data->death != 1)
	{
		printf("%6ld %d %s", time, ph->item + 1, msg);
		return (1);
	}
	return (0);
}

void wait_time(int sleep_time)
{
	unsigned long int cour;

	cour = get_time_in_ms();
	while (get_time_in_ms() < cour + sleep_time)
		usleep(50);
}

int	take_forks(t_ph *ph)
{
	unsigned long int time;

	if (ph->data->death == 1)
		return (0);
	pthread_mutex_lock(&ph->data->forks[min_fork(ph)]);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK) == 0 || min_fork(ph) == max_fork(ph))
	{
		pthread_mutex_unlock(&ph->data->forks[min_fork(ph)]);
		return (0);
	}
	pthread_mutex_lock(&ph->data->forks[max_fork(ph)]);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK) == 0)
	{
		pthread_mutex_unlock(&ph->data->forks[min_fork(ph)]);
		pthread_mutex_unlock(&ph->data->forks[max_fork(ph)]);
		return (0);
	}
	return (1);
}

int eating(t_ph *ph)
{
	unsigned long int	time;

	time = (get_time_in_ms() - ph->data->t_start);
	if (ph->data->death != 1)
		pthread_mutex_lock(&ph->mtx_eat_time);
	if (write_msg(time, ph, MSG_EATING) == 0)
	{
		pthread_mutex_unlock(&ph->data->forks[min_fork(ph)]);
		pthread_mutex_unlock(&ph->data->forks[max_fork(ph)]);
		pthread_mutex_unlock(&ph->mtx_eat_time);
		return (0);
	}
	ph->t_last_eat = get_time_in_ms();
	pthread_mutex_unlock(&ph->mtx_eat_time);
	wait_time(ph->data->t_eat);
//	usleep(ph->data->t_eat * 1000);
	ph->eat_of_time++;
	pthread_mutex_unlock(&ph->data->forks[max_fork(ph)]);
//	usleep(1000);
	pthread_mutex_unlock(&ph->data->forks[min_fork(ph)]);
//	usleep(1000);
	return (1);
}

int	sleeping(t_ph *ph)
{
	unsigned long int	time;

	time = (get_time_in_ms() - ph->data->t_start);
	if (ph->data->death == 1)
		return (0);
	if (write_msg(time, ph, MSG_SLEEPING) == 0)
		return (0);
	wait_time(ph->data->t_sleep);
//	usleep(ph->data->t_sleep * 1000);
	return (1);
}

int	thinking(t_ph *ph)
{
	unsigned long int	time;

	time = (get_time_in_ms() - ph->data->t_start);
	if (ph->data->death == 1)
		return (0);
	if (write_msg(time, ph, MSG_THINKING) == 0)
		return (0);
	return (1);
}



void *check_death(void *arg)
{
	t_ph	*ph;

	ph = (t_ph*)arg;
	pthread_mutex_init(&ph->mtx_eat_time, NULL);
	pthread_mutex_lock(&ph->mtx_eat_time);
	while (get_time_in_ms() - ph->t_last_eat <= (unsigned long int)ph->data->t_die)
	{
		pthread_mutex_unlock(&ph->mtx_eat_time);
		wait_time(10);
		pthread_mutex_lock(&ph->mtx_eat_time);
	}
//	usleep(ph->item * 100);
	if (ph->data->death != 1)
	{
		if (ph->eat_of_time >= ph->data->must_eat && ph->data->must_eat != -1)
			return (NULL);
		if (ph->data->death != 1)
		{
			ph->data->death = 1;
			ph->death = 1;
			printf("%6ld %d %s", get_time_in_ms() - ph->data->t_start, ph->item + 1, MSG_DIED);
		}
	}
	ph->data->death = 1;
	return (NULL);
}

void *philo(void *arg)
{
	t_ph		*ph;
	pthread_t	t_death;

	ph = (t_ph*)arg;
	ph->t_last_eat = ph->data->t_start;
	pthread_create(&t_death, NULL, check_death, &ph[0]);
//	usleep(1000);
	wait_time(1);
	while (ph->data->death != 1)
	{
		if (ph->data->must_eat != -1 && ph->eat_of_time >= ph->data->must_eat)
			break ;
		if (take_forks(ph) == 0)
			break ;
		if (eating(ph) == 0)
			break ;
		if (ph->data->must_eat != -1 && ph->eat_of_time == ph->data->must_eat)
			break ;
		if (sleeping(ph) == 0)
			break ;
		if (thinking(ph) == 0)
			break ;
		wait_time(1);
//		usleep(1000);
	}
	pthread_join(t_death, NULL);
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
		i++;
	}
}

void init_philo(t_data *data)
{
	int	i;
	t_ph *ph;

	i = 1;
	ph = NULL;
	ph = init_philo_sruct(data, ph);
	data->philo = (pthread_t*)malloc(sizeof(pthread_t) * data->num_phil);
	if (!data->philo)
	{
		printf("Error. Pthread - philo init error.\n"); //add destructor
		exit(0);
	}
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i])) //add destructor
			i += 2;
		else
			return ;
	}
	wait_time(data->t_eat);
//	usleep(1000);
	i = 0;
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i])) //add destructor
			i += 2;
		else
			return ;
	}
}

void	end_cleaner(t_data *data)
{
	free(data->forks);
	free(data->philo);
	free(data->ph);
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
	t_data data;

	init_null_data(&data);
	parsing(&data, argc, argv);
	start(&data);
	return 0;
}
