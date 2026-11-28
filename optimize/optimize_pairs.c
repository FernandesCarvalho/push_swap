/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_pairs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 14:03:18 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:36:22 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	can_cancel(t_op first, t_op second);
static int	can_combine(t_op first, t_op second, t_op *result);
static void	optimize_last_pair(t_moves *moves, int *write);
static int	can_simplify(t_op first, t_op second, t_op *result);

static int	can_cancel(t_op first, t_op second)
{
	if (first >= OP_SA && first <= OP_SS && first == second)
		return (1);
	if (first >= OP_RA && first <= OP_RRR
		&& second >= OP_RA && second <= OP_RRR)
	{
		if (first == second - 3 || first == second + 3)
			return (1);
	}
	if ((first == OP_PA && second == OP_PB)
		|| (first == OP_PB && second == OP_PA))
		return (1);
	return (0);
}

static int	can_combine(t_op first, t_op second, t_op *result)
{
	if ((first >= OP_SA && first <= OP_SB
			&& second >= OP_SA && second <= OP_SB)
		|| (first >= OP_RA && first <= OP_RB
			&& second >= OP_RA && second <= OP_RB)
		|| (first >= OP_RRA && first <= OP_RRB
			&& second >= OP_RRA && second <= OP_RRB))
	{
		if (first == second + 1)
		{
			*result = first + 1;
			return (1);
		}
		if (second == first + 1)
		{
			*result = second + 1;
			return (1);
		}
	}
	return (can_simplify(first, second, result));
}

static void	optimize_last_pair(t_moves *moves, int *write)
{
	t_op	result;

	while (*write >= 2)
	{
		if (can_cancel(moves->ops[*write - 1],
				moves->ops[*write - 2]))
		{
			*write -= 2;
		}
		else if (can_combine(moves->ops[*write - 1],
				moves->ops[*write - 2], &result))
		{
			moves->ops[*write - 2] = result;
			(*write)--;
		}
		else
			return ;
	}
}

static int	can_simplify(t_op first, t_op second, t_op *result)
{
	if ((first == OP_SS && second >= OP_SA && second <= OP_SB)
		|| (second == OP_SS && first >= OP_SA && first <= OP_SB))
	{
		*result = 3 - first - second;
		return (1);
	}
	if ((first == OP_RR && second >= OP_RRA && second <= OP_RRB)
		|| (second == OP_RR && first >= OP_RRA && first <= OP_RRB))
	{
		*result = 21 - first - second;
		return (1);
	}
	if ((first == OP_RRR && second >= OP_RA && second <= OP_RB)
		|| (second == OP_RRR && first >= OP_RA && first <= OP_RB))
	{
		*result = 24 - first - second;
		return (1);
	}
	return (0);
}

void	optimize_pairs(t_moves *moves)
{
	int	read;
	int	write;

	if (!moves || !moves->ops || moves->error)
		return ;
	read = 0;
	write = 0;
	while (read < moves->size)
	{
		moves->ops[write] = moves->ops[read];
		write++;
		optimize_last_pair(moves, &write);
		read++;
	}
	moves->size = write;
}
