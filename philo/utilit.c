/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:20:58 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:52:28 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	min_fork(t_ph *ph)
{
	if (ph->l_fork < ph->r_fork)
		return (ph->l_fork);
	else
		return (ph->r_fork);
}

int	max_fork(t_ph *ph)
{
	if (ph->l_fork > ph->r_fork)
		return (ph->l_fork);
	else
		return (ph->r_fork);
}

int	write_msg(unsigned long int time, t_ph *ph, char *msg)
{
	if (get_time_in_ms() - ph->t_last_eat <= \
	(unsigned long int)ph->data->t_die && ph->data->death != 1)
	{
		printf("%6ld %2d %s", time, ph->item + 1, msg);
		return (1);
	}
	return (0);
}
