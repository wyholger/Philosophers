/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:53 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:52:01 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

unsigned long	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	init_time_start(t_data *data)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	data->t_start = (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	wait_time(int sleep_time)
{
	unsigned long int	cour;

	cour = get_time_in_ms();
	while (get_time_in_ms() < cour + sleep_time)
		usleep(50);
}
