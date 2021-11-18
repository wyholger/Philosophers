/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:27 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:46:47 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	error(int error_num, t_data *data)
{
	if (error_num == 1)
	{
		printf("Error. Pthread - philo init error.\n");
		destroy_forks(data);
		end_cleaner(data);
	}
	if (error_num == 2)
	{
		printf("Error. Pthread - philo init error.\n");
		data->death = 1;
		join_thread(data);
		destroy_forks(data);
		end_cleaner(data);
	}
	return (0);
}

void	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_phil)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
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

void	end_cleaner(t_data *data)
{
	free(data->forks);
	free(data->philo);
	free(data->ph);
}
