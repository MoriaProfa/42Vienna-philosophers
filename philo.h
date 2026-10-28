/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:09:58 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 12:17:01 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h> //INT_MAX, INT_MIN
# include <string.h> //memset
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free
# include <unistd.h> //write, usleep
# include <sys/time.h> //gettimeofday
# include <pthread.h> //pthread functions

typedef struct s_philo
{
	int				id;
	size_t			meals;
	pthread_t		thread;
	pthread_mutex_t	*leftfork;
	pthread_mutex_t	*rightfork;
	pthread_mutex_t	meal_lock;
	struct s_sim	*sim;
	long			last_meal;
}	t_philo;

typedef struct s_sim
{
	int				philo_amount;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	size_t			meal_amount;
	int				run_sim_flag;
	int				end_sim_flag;
	int				sim_mutex_inits;
	int				philo_mutex_inits;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	run_sim;
	pthread_mutex_t	end_sim;
	pthread_mutex_t	printing;
	struct timeval	start_time;
}	t_sim;

//utils.c
void	ft_putstr_fd(int fd, char *str);
void	mutex_printf(long timestamp, int id, char *str, t_sim *sim);
void	*ft_calloc(size_t nmemb, size_t size);
long	get_time(t_sim *sim);
void	ft_usleep(long time, t_sim *sim);

//handle_args.c
int		philo_atoi(const char *nptr);
int		check_argv_validity(int argc, char **argv);

//init.c
int		init_sim(t_sim *sim, char **argv);
int		init_philos(t_sim *sim);

//flags.c
void	reset_run_sim_flag(t_sim *sim);
void	set_end_sim_flag(t_sim *sim);
int		get_flag(t_sim *sim, char c);

//routine.c
void	*ph_routine(void *arg);

//eat.c
void	eat(t_philo *philo);

//end.c
int		join_philos(int count, t_sim *sim);
void	cleanup(t_sim *sim);

#endif