/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:30:25 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 02:10:32 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putstr_fd(int fd, char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(fd, str, i);
}

void	mutex_printf(long timestamp, int id, char *str, t_sim *sim)
{
	if (!get_flag(sim, 'r') || get_flag(sim, 'e'))
		return ;
	if (pthread_mutex_lock(&sim->printing) != 0)
		ft_putstr_fd(2, "Printing mutex lock failed\n");
	if (get_flag(sim, 'r') && !get_flag(sim, 'e'))
		printf("%ld %i %s\n", timestamp, id, str);
	if (pthread_mutex_unlock(&sim->printing) != 0)
		ft_putstr_fd(2, "Printing mutex unlock failed\n");
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, nmemb * size);
	return (ptr);
}

long	get_time(t_sim *sim)
{
	struct timeval	end;
	long			seconds;
	long			microseconds;
	long			milliseconds;

	if (gettimeofday(&end, NULL) != 0)
		return (ft_putstr_fd(2, "Timestamp's gettimeofday() failed\n"), -1);
	seconds = end.tv_sec - sim->start_time.tv_sec;
	microseconds = end.tv_usec - sim->start_time.tv_usec;
	milliseconds = (seconds * 1000) + (microseconds / 1000);
	return (milliseconds);
}

void	ft_usleep(long time, t_sim *sim)
{
	long	start;

	start = get_time(sim);
	while (get_time(sim) - start < time)
	{
		if (!get_flag(sim, 'r') || get_flag(sim, 'e'))
			break ;
		if (usleep(500) == -1)
			ft_putstr_fd(2, "Usleep failed\n");
	}
}
