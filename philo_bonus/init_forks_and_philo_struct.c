/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks_and_philo_struct.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:21:21 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:28:37 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	init_null_data(t_data *data)
{
	data->death = 0;
	data->philo = NULL;
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

t_ph	*init_philo_struct(t_data *data)
{
	int		i;
	t_ph	*ph;

	i = 0;
	ph = NULL;
	ph = (t_ph *)malloc(sizeof(t_ph) * data->num_phil);
	if (!ph)
		exit(error(2, data));
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
