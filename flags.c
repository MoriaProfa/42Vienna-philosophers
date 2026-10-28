/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:02:05 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 01:35:35 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	reset_run_sim_flag(t_sim *sim)
{
	if (pthread_mutex_lock(&sim->run_sim) != 0)
		ft_putstr_fd(2, "Run_sim mutex lock failed\n");
	sim->run_sim_flag = 0;
	if (pthread_mutex_unlock(&sim->run_sim) != 0)
		ft_putstr_fd(2, "Run_sim mutex unlock failed\n");
}

void	set_end_sim_flag(t_sim *sim)
{
	if (pthread_mutex_lock(&sim->end_sim) != 0)
		ft_putstr_fd(2, "End_sim mutex lock failed\n");
	sim->end_sim_flag = 1;
	if (pthread_mutex_unlock(&sim->end_sim) != 0)
		ft_putstr_fd(2, "End_sim mutex unlock failed\n");
}

int	get_flag(t_sim *sim, char c)
{
	int	flag;

	flag = -1;
	if (c == 'e')
	{
		if (pthread_mutex_lock(&sim->end_sim) != 0)
			ft_putstr_fd(2, "End_sim mutex lock failed\n");
		flag = sim->end_sim_flag;
		if (pthread_mutex_unlock(&sim->end_sim) != 0)
			ft_putstr_fd(2, "End_sim mutex unlock failed\n");
	}
	else if (c == 'r')
	{
		if (pthread_mutex_lock(&sim->run_sim) != 0)
			ft_putstr_fd(2, "Run_sim mutex lock failed\n");
		flag = sim->run_sim_flag;
		if (pthread_mutex_unlock(&sim->run_sim) != 0)
			ft_putstr_fd(2, "Run_sim mutex unlock failed\n");
	}
	return (flag);
}
