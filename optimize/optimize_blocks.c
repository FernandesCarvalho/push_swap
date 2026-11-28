/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_blocks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 14:02:47 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:44:50 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	is_rotation(t_op op)
{
	return (op >= OP_RA && op <= OP_RRR);
}

static void	count_rotation(t_op op, t_block *block)
{
	if (op == OP_RA || op == OP_RR)
		block->rot_a++;
	if (op == OP_RB || op == OP_RR)
		block->rot_b++;
	if (op == OP_RRA || op == OP_RRR)
		block->rot_a--;
	if (op == OP_RRB || op == OP_RRR)
		block->rot_b--;
}

static void	init_block(t_block *block, int size_a, int size_b)
{
	block->read = 0;
	block->write = 0;
	block->rot_a = 0;
	block->rot_b = 0;
	block->size_a = size_a;
	block->size_b = size_b;
}

static void	process_operation(t_moves *moves, t_block *block)
{
	t_op	op;

	op = moves->ops[block->read];
	normalize_block(block);
	flush_rotations(moves, block);
	if (op == OP_PA)
	{
		block->size_a++;
		block->size_b--;
	}
	else if (op == OP_PB)
	{
		block->size_a--;
		block->size_b++;
	}
	moves->ops[block->write] = op;
	block->write++;
}

void	optimize_blocks(t_moves *moves, int size_a, int size_b)
{
	t_block	block;

	if (!moves || !moves->ops || moves->error)
		return ;
	init_block(&block, size_a, size_b);
	while (block.read < moves->size)
	{
		if (is_rotation(moves->ops[block.read]))
			count_rotation(moves->ops[block.read], &block);
		else
			process_operation(moves, &block);
		block.read++;
	}
	normalize_block(&block);
	flush_rotations(moves, &block);
	moves->size = block.write;
}
