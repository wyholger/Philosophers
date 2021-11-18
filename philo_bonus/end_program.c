/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:21:12 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:26:00 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	end_cleaner(t_data *data)
{
	sem_close(data->sem_forks);
	sem_close(data->sem_eat_time);
	sem_close(data->sem_write);
	free(data->philo);
	free(data->ph);
}

void	join_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_phil)
	{
		pthread_join(data->philo[i], NULL);
		i++;
	}
}

void	sem_disable(void)
{
	sem_unlink("forks");
	sem_unlink("eat_time");
	sem_unlink("write");
}

void	my_sem_close(t_data *data)
{
	sem_close(data->sem_forks);
	sem_close(data->sem_eat_time);
	sem_close(data->sem_write);
}

int	error(int error_num, t_data *data)
{
	if (error_num == 1)
	{
		sem_disable();
		my_sem_close(data);
		printf("Error. Init semaphore forks error!\n");
	}
	if (error_num == 2)
	{
		sem_disable();
		my_sem_close(data);
		printf("Error. Pthread - philo init error.\n");
	}
	if (error_num == 3)
	{
		join_thread(data);
		sem_disable();
		my_sem_close(data);
		printf("Error. Pthread - philo init error.\n");
	}
	return (0);
}
