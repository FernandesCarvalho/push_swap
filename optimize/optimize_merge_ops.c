/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_merge_ops.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:47:53 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:48:56 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	split_operation(t_op op, t_moves *a, t_moves *b)
{
	if (op == OP_SA || op == OP_RA || op == OP_RRA)
		add_move(a, op);
	else if (op == OP_SB || op == OP_RB || op == OP_RRB)
		add_move(b, op);
	else if (op == OP_SS || op == OP_RR || op == OP_RRR)
	{
		add_move(a, op - 2);
		add_move(b, op - 1);
	}
}

static int	can_merge(t_op op_a, t_op op_b)
{
	return (op_b == op_a + 1);
}

static int	take_b(t_moves *a, t_moves *b, int i, int j)
{
	if (i + 1 < a->size
		&& can_merge(a->ops[i + 1], b->ops[j]))
		return (0);
	if (j + 1 < b->size
		&& can_merge(a->ops[i], b->ops[j + 1]))
		return (1);
	return (0);
}

static void	append_remaining(t_moves *src, t_moves *result, int i)
{
	while (i < src->size)
	{
		add_move(result, src->ops[i]);
		i++;
	}
}

void	merge_operations(t_moves *a, t_moves *b, t_moves *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < a->size && j < b->size)
	{
		if (can_merge(a->ops[i], b->ops[j]))
		{
			add_move(result, a->ops[i] + 2);
			i++;
			j++;
		}
		else if (take_b(a, b, i, j))
			add_move(result, b->ops[j++]);
		else
			add_move(result, a->ops[i++]);
	}
	append_remaining(a, result, i);
	append_remaining(b, result, j);
}
