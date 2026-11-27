/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 12:15:10 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:52:37 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	init_merge(t_merge *merge, int initial_size)
{
	if (init_moves(&merge->a))
		return (1);
	if (init_moves(&merge->b))
	{
		free_moves(&merge->a);
		return (1);
	}
	if (init_moves(&merge->result))
	{
		free_moves(&merge->a);
		free_moves(&merge->b);
		return (1);
	}
	merge->read = 0;
	merge->write = 0;
	merge->start = 0;
	merge->size_a = initial_size;
	merge->size_b = 0;
	return (0);
}

static void	free_merge(t_merge *merge)
{
	free_moves(&merge->a);
	free_moves(&merge->b);
	free_moves(&merge->result);
}

static void	process_push(t_moves *moves, t_merge *merge)
{
	t_op	op;

	op = moves->ops[merge->read];
	process_block(merge);
	write_block(moves, merge);
	if (op == OP_PA)
	{
		merge->size_a++;
		merge->size_b--;
	}
	else
	{
		merge->size_a--;
		merge->size_b++;
	}
	moves->ops[merge->write] = op;
	merge->write++;
	merge->a.size = 0;
	merge->b.size = 0;
	merge->start = merge->read + 1;
}

void	optimize_merge(t_moves *moves, int initial_size)
{
	t_merge	merge;
	t_op	op;

	if (!moves || !moves->ops || moves->error)
		return ;
	if (init_merge(&merge, initial_size))
		return ;
	while (merge.read < moves->size)
	{
		op = moves->ops[merge.read];
		if (op != OP_PA && op != OP_PB)
			split_operation(op, &merge.a, &merge.b);
		else
			process_push(moves, &merge);
		merge.read++;
	}
	process_block(&merge);
	write_block(moves, &merge);
	moves->size = merge.write;
	free_merge(&merge);
}
