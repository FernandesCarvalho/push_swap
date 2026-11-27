/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_stats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:15:17 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 11:40:59 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	put_str_err(const char *s)
{
	if (!s)
		return ;
	write(2, s, ft_strlen(s));
}

t_ops	*get_ops_stats(void)
{
	static t_ops	stats;

	return (&stats);
}

void	record_op(int *op_count)
{
	if (!op_count)
		return ;
	(*op_count)++;
	get_ops_stats()->total++;
}

void	put_nbr_err(long n)
{
	char	c;

	if (n < 0)
	{
		write(2, "-", 1);
		n = -n;
	}
	if (n >= 10)
		put_nbr_err(n / 10);
	c = (char)((n % 10) + '0');
	write(2, &c, 1);
}
