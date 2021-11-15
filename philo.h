//
// Created by wyholger on 27.10.2021.
//

#ifndef PHILOSOPHERS_PHILO_H
#define PHILOSOPHERS_PHILO_H
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
struct s_philosopher;
typedef struct s_data
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

#endif //PHILOSOPHERS_PHILO_H
