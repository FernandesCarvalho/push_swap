/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_stats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:15:17 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 16:26:12 by fercarva         ###   ########.fr       */
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
void	count_moves(t_moves *moves)
{
	t_ops	*stats;
	int		*counts[11];
	int		i;

	stats = get_ops_stats();
	counts[OP_SA] = &stats->sa;
	counts[OP_SB] = &stats->sb;
	counts[OP_SS] = &stats->ss;
	counts[OP_PA] = &stats->pa;
	counts[OP_PB] = &stats->pb;
	counts[OP_RA] = &stats->ra;
	counts[OP_RB] = &stats->rb;
	counts[OP_RR] = &stats->rr;
	counts[OP_RRA] = &stats->rra;
	counts[OP_RRB] = &stats->rrb;
	counts[OP_RRR] = &stats->rrr;
	i = 0;
	while (i < moves->size)
	{
		record_op(counts[moves->ops[i]]);
		i++;
	}
}