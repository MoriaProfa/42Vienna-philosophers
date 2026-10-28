/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:33:25 by astehl            #+#    #+#             */
/*   Updated: 2025/07/22 23:47:13 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_philos(int amount, t_sim *sim)
{
	int	i;

	i = 0;
	if (amount < sim->philo_amount)
		ft_putstr_fd(2, "Pthread create failed for a philo\n");
	set_end_sim_flag(sim);
	while (i < amount)
	{
		if (pthread_join(sim->philos[i].thread, NULL) != 0)
			return (ft_putstr_fd(2, "Philo pthread join failed!\n"), 0);
		i++;
	}
	return (1);
}

static void	destroy_fork_mutexes(t_sim *sim)
{
	int	i;
	int	initialized_fork_mutexes;

	i = 0;
	if (sim->sim_mutex_inits >= 0 && sim->sim_mutex_inits < sim->philo_amount)
		initialized_fork_mutexes = sim->sim_mutex_inits;
	else
		initialized_fork_mutexes = sim->philo_amount;
	while (i < initialized_fork_mutexes)
	{
		if (pthread_mutex_destroy(&sim->forks[i]) != 0)
			ft_putstr_fd(2, "Mutex destroy of a fork failed\n");
		i++;
	}
}

static void	destroy_sim_and_print_mutexes(t_sim *sim)
{
	if (sim->sim_mutex_inits < -1)
	{
		if (pthread_mutex_destroy(&sim->run_sim) != 0)
			ft_putstr_fd(2, "Run_sim mutex destroy failed\n");
	}
	if (sim->sim_mutex_inits < -2)
	{
		if (pthread_mutex_destroy(&sim->end_sim) != 0)
			ft_putstr_fd(2, "End_sim mutex destroy failed\n");
	}
	if (sim->sim_mutex_inits < -3)
	{
		if (pthread_mutex_destroy(&sim->printing) != 0)
			ft_putstr_fd(2, "Printing mutex destroy failed\n");
	}
}

static void	destroy_philo_meal_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->philo_mutex_inits)
	{
		if (pthread_mutex_destroy(&sim->philos[i].meal_lock) != 0)
			ft_putstr_fd(2, "Mutex destroy of a philos meal_lock failed\n");
		i++;
	}
}

void	cleanup(t_sim *sim)
{
	if (sim->forks)
	{
		destroy_fork_mutexes(sim);
		if (sim->sim_mutex_inits < 0)
			destroy_sim_and_print_mutexes(sim);
		free(sim->forks);
		sim->forks = NULL;
	}
	if (sim->philos)
	{
		destroy_philo_meal_mutexes(sim);
		free(sim->philos);
		sim->philos = NULL;
	}
}
