/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:38 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:50:43 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*check_death(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	pthread_mutex_init(&ph->mtx_eat_time, NULL);
	pthread_mutex_lock(&ph->mtx_eat_time);
	while (get_time_in_ms() - ph->t_last_eat <= \
	(unsigned long int)ph->data->t_die)
	{
		pthread_mutex_unlock(&ph->mtx_eat_time);
		wait_time(1);
		pthread_mutex_lock(&ph->mtx_eat_time);
	}
	if (ph->data->death != 1)
	{
		if (ph->eat_of_time >= ph->data->must_eat && ph->data->must_eat != -1)
			return (NULL);
		if (ph->data->death != 1)
		{
			ph->data->death = 1;
			ph->death = 1;
			printf("%6ld %d %s", get_time_in_ms() - \
			ph->data->t_start, ph->item + 1, MSG_DIED);
		}
	}
	return (NULL);
}

void	*philo(void *arg)
{
	t_ph		*ph;
	pthread_t	t_death;

	ph = (t_ph *)arg;
	ph->t_last_eat = ph->data->t_start;
	pthread_create(&t_death, NULL, check_death, &ph[0]);
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
	}
	pthread_join(t_death, NULL);
	return (NULL);
}

void	init_philo(t_data *data)
{
	int		i;
	t_ph	*ph;

	i = 1;
	ph = init_philo_struct(data);
	data->philo = (pthread_t *)malloc(sizeof(pthread_t) * data->num_phil);
	if (!data->philo)
		exit(error(1, data));
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i]))
			i += 2;
		else
			exit(error(2, data));
	}
	wait_time(data->t_eat);
	i = 0;
	while (i < data->num_phil)
	{
		if (!pthread_create(&data->philo[i], NULL, philo, &ph[i]))
			i += 2;
		else
			exit(error(2, data));
	}
}

void	start(t_data *data)
{
	init_forks(data);
	init_time_start(data);
	init_philo(data);
	join_thread(data);
	destroy_forks(data);
	end_cleaner(data);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	init_null_data(&data);
	parsing(&data, argc, argv);
	start(&data);
	return (0);
}
