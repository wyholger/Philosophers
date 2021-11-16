#include "philo_bonus.h"

size_t	ft_strlen(const char *str)
{
	unsigned long int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}


int	number_depth(long int long_n, int single)
{
	int		count;

	count = 0;
	if (long_n == 0)
		return (count + 1);
	while (long_n > 0)
	{
		long_n = long_n / 10;
		count++;
	}
	if (single < 0)
		count++;
	return (count);
}

int	singl (long int long_n)
{
	int	single;

	single = 1;
	if (long_n < 0)
		single = -1;
	return (single);
}

char	*resulting(char *rezult, int count, long int long_n, int single)
{
	rezult[count] = '\0';
	if (long_n == 0)
		rezult[0] = '0';
	while (long_n > 0)
	{
		rezult[--count] = (long_n % 10) + '0';
		long_n = long_n / 10;
	}
	if (single < 0)
		rezult[--count] = '-';
	return (rezult);
}

char	*ft_itoa(int n)
{
	long int	long_n;
	char		*rezult;
	int			count;
	int			single;

	long_n = (long int)n;
	single = singl(long_n);
	if (single < 0)
		long_n = long_n * -1;
	count = number_depth(long_n, single);
	rezult = malloc ((count + 1) * sizeof(char));
	if (!rezult)
		return (NULL);
	rezult = resulting(rezult, count, long_n, single);
	return (rezult);
}

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
		i++;
	}
	data->ph = ph;
	return (ph);
}

void wait_time(int sleep_time)
{
	unsigned long int cour;

	cour = get_time_in_ms();
	while (get_time_in_ms() < cour + sleep_time)
		usleep(50);
}

int write_msg(unsigned long int time, t_ph *ph, char *msg, int len_msg)
{
	char	*char_time;
	char	*char_num_phil;


	if (ph->data->death != 1) //get_time_in_ms() - ph->t_last_eat <= (unsigned long int)ph->data->t_die && 
	{
		if ((msg == MSG_DIED && ph->death == 1 && ph->data->death != 1) || (msg != MSG_DIED && ph->data->death != 1))
		{
			if (msg == MSG_DIED)
				ph->data->death = 1;
			char_time = ft_itoa((int)time);
			char_num_phil = ft_itoa(ph->item + 1);
			sem_wait(ph->data->sem_write);
			write(1, &char_time[0], ft_strlen(char_time));
			write(1, " ", 1);
			write(1, &char_num_phil[0], ft_strlen(char_num_phil));
			write(1, &msg[0], len_msg);
			sem_post(ph->data->sem_write);
			free(char_time);
			free(char_num_phil);
		}
		return (1);
	}
	return (0);
}

void	sem_disable(void)
{
	sem_unlink("forks");
	sem_unlink("eat_time");
	sem_unlink("write");
}

void	init_null_data(t_data *data)
{
	data->death = 0;
	data->philo = NULL;
	data->death_flag = 0;
	sem_disable();
	data->sem_eat_time = sem_open("eat_time", O_CREAT, 0660, 1);
	data->sem_write = sem_open("write", O_CREAT, 0660, 1);
}

int	init_forks(t_data *data)
{
	data->sem_forks = sem_open("forks", O_CREAT, 0660, data->num_phil);
	if (data->sem_forks == SEM_FAILED)
		return (0);
	return (1);
}

int error(int error_num)
{
	if (error_num == 1)
	{
		sem_disable();
		printf("Error. Init semaphore forks error!\n");
	}
	return (0);
}

void *check_death(void *arg)
{
	t_ph	*ph;
	
	ph = (t_ph*)arg;
	sem_wait(ph->data->sem_eat_time);
	while (get_time_in_ms() - ph->t_last_eat <= (unsigned long int)ph->data->t_die && ph->data->death != 1)
	{
		sem_post(ph->data->sem_eat_time);
		wait_time(1);
//		usleep(100);
		sem_wait(ph->data->sem_eat_time);
	}
	sem_post(ph->data->sem_eat_time);
//	usleep(ph->item * 100);
	if (ph->data->death != 1)
	{
		if (ph->eat_of_time >= ph->data->must_eat && ph->data->must_eat != -1)
			return (NULL);
		if (ph->data->death != 1)
		{
			ph->death = 1;
			write_msg(get_time_in_ms() - ph->data->t_start, ph, MSG_DIED, SIZE_MSG_DIED);
			ph->data->death = 1;
		}
	}
	ph->data->death = 1;
	return (NULL);
}

int	take_forks(t_ph *ph)
{
	unsigned long int time;
	
	if (ph->data->death == 1)
		return (0);
	sem_wait(ph->data->sem_forks);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK, SIZE_MSG_TAKEN_FORK) == 0 || ph->data->num_phil == 1)
	{
		sem_post(ph->data->sem_forks);
		return (0);
	}
	sem_wait(ph->data->sem_forks);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK, SIZE_MSG_TAKEN_FORK) == 0)
	{
		sem_post(ph->data->sem_forks);
		sem_post(ph->data->sem_forks);
		return (0);
	}
	return (1);
}

int eating(t_ph *ph)
{
	unsigned long int	time;
	
	time = (get_time_in_ms() - ph->data->t_start);
	sem_wait(ph->data->sem_eat_time);
	if (write_msg(time, ph, MSG_EATING, SIZE_MSG_EATING) == 0)
	{
		sem_post(ph->data->sem_forks);
		sem_post(ph->data->sem_forks);
		sem_post(ph->data->sem_eat_time);
		return (0);
	}
	ph->t_last_eat = get_time_in_ms();
	sem_post(ph->data->sem_eat_time);
	wait_time(ph->data->t_eat);
//	usleep(ph->data->t_eat * 1000);
	ph->eat_of_time++;
	sem_post(ph->data->sem_forks);
	sem_post(ph->data->sem_forks);
	return (1);
}

int	sleeping(t_ph *ph)
{
	unsigned long int	time;
	
	time = (get_time_in_ms() - ph->data->t_start);
	if (ph->data->death == 1)
		return (0);
	if (write_msg(time, ph, MSG_SLEEPING, SIZE_MSG_SLEEPING) == 0)
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
	if (write_msg(time, ph, MSG_THINKING, SIZE_MSG_SLEEPING) == 0)
		return (0);
	return (1);
}


void *philo(void *arg)
{
	t_ph		*ph;
	pthread_t	t_death;
	
	ph = (t_ph*)arg;
	ph->t_last_eat = ph->data->t_start;
	pthread_create(&t_death, NULL, check_death, &ph[0]);
//	usleep(1000);
	while (ph->data->death != 1)
	{
		if (ph->data->must_eat != -1 && ph->eat_of_time >= ph->data->must_eat)
			break ;
		if (take_forks(ph) == 0)
			break ;
		if (eating(ph) == 0)
			break ;
		if (sleeping(ph) == 0)
			break ;
		if (ph->data->must_eat != -1 && ph->eat_of_time == ph->data->must_eat)
			break ;
		if (thinking(ph) == 0)
			break ;
		wait_time(1);
//		usleep(1000);
	}
	pthread_join(t_death, NULL);
	return (NULL);
}

int	init_philo(t_data *data)
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
		exit(0);
	}
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i]))
			i += 2;
		else
			return (0);
	}
	wait_time(data->t_eat);
//	usleep(1000);
	i = 1;
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i]))
			i += 2;
		else
			return (0);
	}
	return (1);
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
}

void	end_cleaner(t_data *data)
{
	sem_close(data->sem_forks);
	sem_close(data->sem_eat_time);
	free(data->philo);
	free(data->ph);
}

int start(t_data *data)
{
	if (init_forks(data) == 0)
		return (error(1));
	init_time_start(data);
	init_philo(data);
	join_thread(data);
	end_cleaner(data);
	return (1);
}

int main(int argc, char *argv[])
{
	t_data data;
	
	init_null_data(&data);
	parsing(&data, argc, argv);
	start(&data);
}
