/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wyholger <wyholger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 21:54:34 by wyholger          #+#    #+#             */
/*   Updated: 2021/11/18 21:56:20 by wyholger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include "philo.h"
# define MSG_TAKEN_FORK			"\033[33m has taken a fork\033[0m\n"
# define MSG_EATING				"\033[32m is eating\033[0m\n"
# define MSG_SLEEPING			"\033[35m is sleeping\033[0m\n"
# define MSG_THINKING			"\033[36m is thinking\033[0m\n"
# define MSG_DIED				"\033[31m died\033[0m\n"

struct	s_philosopher;
struct	s_data;
typedef struct	s_data
{
	int						num_phil;
	int						t_die;
	int						t_eat;
	int						t_sleep;
	int 					must_eat;
	pthread_mutex_t			*forks;
	pthread_mutex_t			*can_eat;
	pthread_mutex_t			*poel;
	pthread_mutex_t			*need_eat;
	pthread_t				*philo;
	pthread_t				*waiter;
	struct s_philosopher	*ph;
	unsigned long int 		t_start;
	int 					*priority;
	int 					death;
	int 					death_flag;
}				t_data;
typedef struct s_philosopher
{
	int					item;
	int					l_fork;
	int 				r_fork;
	int 				l_phil;
	int 				r_phil;
	int 				death;
	int 				eat_of_time;
	unsigned long int	t_last_eat;
	t_data				*data;
	pthread_mutex_t		mtx_local_death;
	pthread_mutex_t		mtx_eat_time;
}				t_ph;
void				parsing(t_data *data, int argc, char *argv[]);
void				check_argc(int argc);
void				check_valid_argv(char *argv[], int argc);
int					is_positive_digit(int character);
int					ft_atoi(const char *str);
unsigned long		get_time_in_ms(void);
void				init_time_start(t_data *data);
void				wait_time(int sleep_time);
void				init_forks(t_data *data);
void				init_null_data(t_data *data);
t_ph				*init_philo_struct(t_data *data);
void				end_cleaner(t_data *data);
void				join_thread(t_data *data);
void				destroy_forks(t_data *data);
int					error(int error_num, t_data *data);
int					min_fork(t_ph *ph);
int					max_fork(t_ph *ph);
int					write_msg(unsigned long int time, t_ph *ph, char *msg);
int					take_forks(t_ph *ph);
int					eating(t_ph *ph);
int					sleeping(t_ph *ph);
int					thinking(t_ph *ph);
void				start(t_data *data);
void				init_philo(t_data *data);
void				*philo(void *arg);
void				*check_death(void *arg);
#endif //PHILO_H