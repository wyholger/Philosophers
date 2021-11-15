//
// Created by wyholger on 11.11.2021.
//

#ifndef PHILO_3_BONUS_PHILO_BONUS_H
#define PHILO_3_BONUS_PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include "philo_bonus.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# define MSG_TAKEN_FORK			"\033[33m has taken a fork\033[0m\n"
# define SIZE_MSG_TAKEN_FORK	27
# define MSG_EATING				"\033[32m is eating\033[0m\n"
# define SIZE_MSG_EATING		20
# define MSG_SLEEPING			"\033[35m is sleeping\033[0m\n"
# define SIZE_MSG_SLEEPING		22
# define MSG_THINKING			"\033[36m is thinking\033[0m\n"
# define SIZE_MSG_THINKING		22
# define MSG_DIED				"\033[31m died\033[0m\n"
# define SIZE_MSG_DIED			15
struct s_philosopher;
typedef struct s_data
{
	int						num_phil;
	int						t_die;
	int						t_eat;
	int						t_sleep;
	int 					must_eat;
	sem_t					*sem_forks;
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
	sem_t				    *sem_eat_time;
	sem_t					*sem_write;
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
	sem_t				*sem_eat_time;
}				t_ph;

#endif //PHILO_3_BONUS_PHILO_BONUS_H
