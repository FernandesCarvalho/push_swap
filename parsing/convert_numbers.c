/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_numbers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 11:30:11 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/21 20:14:54 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	check_overflow(long res, int sign, int digit)
{
	if (sign == 1 && (res > (INT_MAX - digit) / 10))
		return (1);
	if (sign == -1 && (-res < (INT_MIN + digit) / 10))
		return (1);
	return (0);
}

int	convert_numbers(char *str, int *result)
{
	long	res;
	int		sign;
	int		digit;

	if (!str || !result)
		return (0);
	res = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -sign;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit((unsigned char)*str))
			return (0);
		digit = *str++ - '0';
		if (check_overflow(res, sign, digit))
			return (0);
		res = (res * 10) + digit;
	}
	*result = (int)(res * sign);
	return (1);
}
