/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:33:25 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 01:55:37 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lock_forks(t_philo *philo, pthread_mutex_t **first, \
	pthread_mutex_t **second)
{
	if (get_flag(philo->sim, 'e') || !get_flag(philo->sim, 'r'))
		return (0);
	if (pthread_mutex_lock(*first) != 0)
		ft_putstr_fd(2, "First fork mutex lock failed for a philo\n");
	if (get_flag(philo->sim, 'e') || !get_flag(philo->sim, 'r'))
	{
		if (pthread_mutex_unlock(*first) != 0)
			ft_putstr_fd(2, "Left fork of a philo couldn't be unlocked\n");
		return (0);
	}
	mutex_printf(get_time(philo->sim), philo->id, "has taken a fork", \
		philo->sim);
	if (pthread_mutex_lock(*second) != 0)
		ft_putstr_fd(2, "Second fork mutex lock failed for a philo\n");
	if (get_flag(philo->sim, 'e') || !get_flag(philo->sim, 'r'))
	{
		if (pthread_mutex_unlock(*first) != 0)
			ft_putstr_fd(2, "Left fork of a philo couldn't be unlocked\n");
		if (pthread_mutex_unlock(*second) != 0)
			ft_putstr_fd(2, "Right fork of a philo couldn't be unlocked\n");
		return (0);
	}
	mutex_printf(get_time(philo->sim), philo->id, "has taken a fork", \
		philo->sim);
	return (1);
}

static void	update_meal(t_philo *philo, int update_count)
{
	if (pthread_mutex_lock(&philo->meal_lock) != 0)
		ft_putstr_fd(2, "Meal mutex lock failed for a philo\n");
	if (update_count)
		philo->meals++;
	else
		philo->last_meal = get_time(philo->sim);
	if (pthread_mutex_unlock(&philo->meal_lock) != 0)
		ft_putstr_fd(2, "Meal mutex unlock failed for a philo\n");
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->leftfork < philo->rightfork)
	{
		first = philo->leftfork;
		second = philo->rightfork;
	}
	else
	{
		first = philo->rightfork;
		second = philo->leftfork;
	}
	if (!lock_forks(philo, &first, &second))
		return ;
	mutex_printf(get_time(philo->sim), philo->id, "is eating", philo->sim);
	update_meal(philo, 0);
	ft_usleep(philo->sim->time_to_eat, philo->sim);
	if (pthread_mutex_unlock(first) != 0)
		ft_putstr_fd(2, "Left fork of a philo couldn't be unlocked\n");
	if (pthread_mutex_unlock(second) != 0)
		ft_putstr_fd(2, "Right fork of a philo couldn't be unlocked\n");
	update_meal(philo, 1);
}
