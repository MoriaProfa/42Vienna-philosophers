/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:44:05 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 01:46:27 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_single_philo(t_philo *philo)
{
	if (pthread_mutex_lock(philo->leftfork) != 0)
		ft_putstr_fd(2, "Left fork mutex lock failed for single philo\n");
	mutex_printf(get_time(philo->sim), philo->id, "has taken a fork", \
		philo->sim);
	if (usleep(philo->sim->time_to_die * 1000) == -1)
		ft_putstr_fd(2, "Usleep failed for single philo\n");
	if (pthread_mutex_unlock(philo->leftfork) != 0)
		ft_putstr_fd(2, "Left fork of single philo couldn't be unlocked\n");
}

static void	determine_routine_start(t_philo *philo)
{
	if (philo->meals == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(philo->sim->time_to_eat, philo->sim);
		if (philo->id == 1 && philo->sim->philo_amount % 2 != 0)
			ft_usleep(philo->sim->time_to_eat * 2, philo->sim);
	}
	if (philo->sim->philo_amount % 2 != 0 && philo->meals != 0)
		ft_usleep(philo->sim->time_to_eat, philo->sim);
}

void	*ph_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!get_flag(philo->sim, 'r') && !get_flag(philo->sim, 'e'))
	{
		if (usleep(1) == -1)
			ft_putstr_fd(2, "Usleep before routine start failed\n");
	}
	while (get_flag(philo->sim, 'r') && !get_flag(philo->sim, 'e'))
	{
		if (philo->sim->philo_amount == 1)
			return (handle_single_philo(philo), NULL);
		determine_routine_start(philo);
		eat(philo);
		if (get_flag(philo->sim, 'r') && !get_flag(philo->sim, 'e'))
		{
			mutex_printf(get_time(philo->sim), philo->id, "is sleeping", \
				philo->sim);
			ft_usleep(philo->sim->time_to_sleep, philo->sim);
		}
		if (get_flag(philo->sim, 'r') && !get_flag(philo->sim, 'e'))
			mutex_printf(get_time(philo->sim), philo->id, "is thinking", \
				philo->sim);
	}
	return (NULL);
}
