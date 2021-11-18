/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks_and_philo_struct.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:32 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:47:57 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	init_null_data(t_data *data)
{
	data->death = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->death_flag = 0;
}

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) \
	* data->num_phil);
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

t_ph	*init_philo_struct(t_data *data)
{
	int		i;
	t_ph	*ph;

	i = 0;
	ph = NULL;
	ph = (t_ph *)malloc(sizeof(t_ph) * data->num_phil);
	if (!ph)
		exit(error(1, data));
	while (i < data->num_phil)
	{
		ph[i].eat_of_time = 0;
		ph[i].death = 0;
		ph[i].data = data;
		ph[i].item = i;
		ph[i].l_fork = i;
		if (i + 1 != data->num_phil)
			ph[i].r_fork = i + 1;
		else
			ph[i].r_fork = 0;
		i++;
	}
	data->ph = ph;
	return (ph);
}
