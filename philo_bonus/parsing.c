/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:21:29 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:42:09 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	parsing(t_data *data, int argc, char *argv[])
{
	check_argc(argc);
	check_valid_argv(argv, argc);
	data->num_phil = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
}

void	check_argc(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("Error, number arguments error\n");
		exit (0);
	}
}

void	check_valid_argv(char *argv[], int argc)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < argc)
	{
		while (argv[i][j])
		{
			if (is_positive_digit((int)argv[i][j]))
				j++;
			else
			{
				printf("Error. Not valid argument.\n");
				exit (0);
			}
		}
		j = 0;
		i++;
	}
}

int	is_positive_digit(int character)
{
	if ((character >= '0' && character <= '9') || character == '+')
	{
		return (1);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int					i;
	unsigned long int	rezult;
	int					single;

	i = 0;
	rezult = 0;
	single = 1;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n')
		|| (str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			single = single * -1;
	}
	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		rezult = (rezult * 10) + (str[i] - '0');
		i++;
		if (rezult > 9223372036854775807 && single > 0)
			return (-1);
		if (rezult > 9223372036854775807 && single < 0)
			return (0);
	}
	return ((int)rezult * single);
}
