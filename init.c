/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:06:26 by astehl            #+#    #+#             */
/*   Updated: 2025/07/22 23:48:11 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_sim_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->philo_amount)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (ft_putstr_fd(2, "A fork's mutex init failed\n"), i);
		i++;
	}
	if (pthread_mutex_init(&sim->run_sim, NULL) != 0)
		return (ft_putstr_fd(2, "Run_sim mutex init failed\n"), -1);
	if (pthread_mutex_init(&sim->end_sim, NULL) != 0)
		return (ft_putstr_fd(2, "End_sim mutex init failed\n"), -2);
	if (pthread_mutex_init(&sim->printing, NULL) != 0)
		return (ft_putstr_fd(2, "Printing mutex init failed\n"), -3);
	return (-42);
}

int	init_sim(t_sim *sim, char **argv)
{
	sim->philo_amount = philo_atoi(argv[1]);
	sim->time_to_die = philo_atoi(argv[2]);
	sim->time_to_eat = philo_atoi(argv[3]);
	sim->time_to_sleep = philo_atoi(argv[4]);
	if (argv[5])
		sim->meal_amount = philo_atoi(argv[5]);
	else
		sim->meal_amount = -1;
	sim->run_sim_flag = 0;
	sim->end_sim_flag = 0;
	sim->philo_mutex_inits = -1;
	sim->philos = ft_calloc(sim->philo_amount, sizeof(t_philo));
	if (!sim->philos)
		return (ft_putstr_fd(2, "Philos malloc failed\n"), 0);
	sim->forks = ft_calloc(sim->philo_amount, sizeof(pthread_mutex_t));
	if (!sim->forks)
		return (ft_putstr_fd(2, "Forks malloc failed\n"), 0);
	sim->sim_mutex_inits = init_sim_mutexes(sim);
	if (sim->sim_mutex_inits != -42)
		return (0);
	return (1);
}

static int	init_philo_data(t_sim *sim, int i)
{
	sim->philos[i].id = i + 1;
	sim->philos[i].meals = 0;
	sim->philos[i].sim = sim;
	sim->philos[i].leftfork = &sim->forks[i];
	sim->philos[i].rightfork = &sim->forks[(i + 1) % sim->philo_amount];
	sim->philos[i].last_meal = 0;
	sim->philo_mutex_inits = i;
	if (pthread_mutex_init(&sim->philos[i].meal_lock, NULL) != 0)
	{
		ft_putstr_fd(2, "Meal_lock mutex init failed for a philo\n");
		return (0);
	}
	return (1);
}

int	init_philos(t_sim *sim)
{
	int	i;

	i = 0;
	if (gettimeofday(&sim->start_time, NULL) != 0)
		return (ft_putstr_fd(2, "Gettimeofday() for sim start failed\n"), 0);
	if (pthread_mutex_lock(&sim->run_sim) != 0)
		ft_putstr_fd(2, "Run_sim mutex lock failed\n");
	while (i < sim->philo_amount)
	{
		if (!init_philo_data(sim, i))
			return (0);
		if (pthread_create(&sim->philos[i].thread, NULL, &ph_routine, \
			(void *)&sim->philos[i]) != 0)
		{
			if (pthread_mutex_unlock(&sim->run_sim) != 0)
				ft_putstr_fd(2, "Run_sim mutex unlock failed\n");
			join_philos(i, sim);
			return (0);
		}
		i++;
	}
	sim->run_sim_flag = 1;
	if (pthread_mutex_unlock(&sim->run_sim) != 0)
		ft_putstr_fd(2, "Run_sim mutex unlock failed\n");
	return (1);
}
