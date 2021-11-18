/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:21:07 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:25:11 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	take_forks(t_ph *ph)
{
	unsigned long int	time;

	if (ph->data->death == 1)
		return (0);
	sem_wait(ph->data->sem_forks);
	time = (get_time_in_ms() - ph->data->t_start);
	if (write_msg(time, ph, MSG_TAKEN_FORK, SIZE_MSG_TAKEN_FORK) == 0 || \
	ph->data->num_phil == 1)
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

int	eating(t_ph *ph)
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
