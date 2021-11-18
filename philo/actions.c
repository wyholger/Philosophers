/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:15 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:46:03 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	take_forks(t_ph *ph)
{
	unsigned long int	time;

	if (ph->data->death == 1)
		return (0);
	pthread_mutex_lock(&ph->data->forks[min_fork(ph)]);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK) == 0 \
	|| min_fork(ph) == max_fork(ph))
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

int	eating(t_ph *ph)
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
	ph->eat_of_time++;
	pthread_mutex_unlock(&ph->data->forks[max_fork(ph)]);
	pthread_mutex_unlock(&ph->data->forks[min_fork(ph)]);
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
