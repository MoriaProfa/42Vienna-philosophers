/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:33:51 by astehl            #+#    #+#             */
/*   Updated: 2025/07/17 01:56:28 by astehl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	signcheck(char c)
{
	if (c == '-')
		return (-1);
	return (1);
}

static int	check_overflow(int result, char digit, int sign)
{
	if (result > INT_MAX / 10 || \
		(result == INT_MAX / 10 && \
		((sign == 1 && digit - '0' > INT_MAX % 10) || \
		(sign == -1 && digit - '0' > -(INT_MIN % 10)))))
		return (1);
	return (0);
}

int	philo_atoi(const char *nptr)
{
	int	result;
	int	i;
	int	sign;

	result = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		sign = signcheck(nptr[i++]);
	if (sign == -1)
		return (-1);
	while (nptr[i] == '0')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (check_overflow(result, nptr[i], sign))
			return (-1);
		result = result * 10 + (nptr[i] - '0');
		if (nptr[i + 1] != '\0' && (nptr[i + 1] < '0' || nptr[i + 1] > '9'))
			return (-1);
		i++;
	}
	return (result * sign);
}

int	check_argv_validity(int argc, char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && i < argc)
	{
		if (philo_atoi(argv[i]) < 1)
			return (0);
		i++;
	}
	return (1);
}
