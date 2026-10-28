/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:30:05 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 12:52:34 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//todos:
//eat.c is still too long

//tests/tools:
//./philo 3 600 200 200 10 > test.txt <-- in ein testfile ausgeben
//./philo 3 600 200 200 10 | tee test.txt <-- im terminal drucken 
//UND in ein testfile ausgeben

//test with  ./philo 200 800 200 90 and ./philo 200 550 200 20

//(ulimit -v 120000; valgrind --tool=helgrind ./philo 4 600 200 200 5)

//valgrind --tool=helgrind ./philo 4 410 200 200 2 <--check all errors
//--tool=drd

//funcheck --test-functions='malloc,free'

static void	check_stop(long elapsed_time, int count, int i, t_sim *sim)
{
	if (elapsed_time >= sim->time_to_die)
	{
		set_end_sim_flag(sim);
		reset_run_sim_flag(sim);
		if (pthread_mutex_lock(&sim->printing) != 0)
			ft_putstr_fd(2, "Printing mutex lock failed\n");
		printf("%ld %i died\n", get_time(sim), sim->philos[i].id);
		if (pthread_mutex_unlock(&sim->printing) != 0)
			ft_putstr_fd(2, "Printing mutex unlock failed\n");
		return ;
	}
	if (count == sim->philo_amount)
		reset_run_sim_flag(sim);
}

static void	monitor(t_sim *sim)
{
	int		i;
	int		count;
	long	elapsed_time;
	size_t	meals;

	while (get_flag(sim, 'r'))
	{
		i = 0;
		count = 0;
		while (get_flag(sim, 'r') && i < sim->philo_amount)
		{
			if (pthread_mutex_lock(&sim->philos[i].meal_lock) != 0)
				ft_putstr_fd(2, "Meal mutex lock failed for a philo\n");
			meals = sim->philos[i].meals;
			elapsed_time = get_time(sim) - sim->philos[i].last_meal;
			if (pthread_mutex_unlock(&sim->philos[i].meal_lock) != 0)
				ft_putstr_fd(2, "Meal mutex unlock failed for a philo\n");
			if (meals >= sim->meal_amount)
				count++;
			check_stop(elapsed_time, count, i, sim);
			i++;
		}
		if (get_flag(sim, 'r') && usleep(100) == -1)
			ft_putstr_fd(2, "Usleep in the end of monitor failed\n");
	}
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (argc < 5 || argc > 6)
		return (ft_putstr_fd(2, "Too many/less arguments!\n"), 1);
	if (!check_argv_validity(argc, argv))
		return (ft_putstr_fd(2, "Invalid argument(s)!\n"), 1);
	if (!init_sim(&sim, argv))
		return (cleanup(&sim), 1);
	if (!init_philos(&sim))
		return (cleanup(&sim), 1);
	monitor(&sim);
	join_philos(sim.philo_amount, &sim);
	return (cleanup(&sim), 0);
}
