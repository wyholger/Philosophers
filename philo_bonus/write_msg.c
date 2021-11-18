/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:21:37 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:44:14 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	write_msg(unsigned long int time, t_ph *ph, char *msg, int len_msg)
{
	char	*char_time;
	char	*char_num_phil;

	if (ph->data->death != 1)
	{
		if ((&msg[0] == &MSG_DIED[0] && ph->death == 1 && ph->data->death \
		!= 1) || (&msg[0] != &MSG_DIED[0] && ph->data->death != 1))
		{
			if (&msg[0] == &MSG_DIED[0])
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
