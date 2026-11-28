/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_flush.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:43:28 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:43:36 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	flush_combined(t_moves *moves, t_block *block)
{
	while (block->rot_a > 0 && block->rot_b > 0)
	{
		moves->ops[block->write] = OP_RR;
		block->write++;
		block->rot_a--;
		block->rot_b--;
	}
	while (block->rot_a < 0 && block->rot_b < 0)
	{
		moves->ops[block->write] = OP_RRR;
		block->write++;
		block->rot_a++;
		block->rot_b++;
	}
}

static void	flush_single(t_moves *moves, int *write,
	int rotation, t_op forward)
{
	while (rotation > 0)
	{
		moves->ops[*write] = forward;
		(*write)++;
		rotation--;
	}
	while (rotation < 0)
	{
		moves->ops[*write] = forward + 3;
		(*write)++;
		rotation++;
	}
}

void	flush_rotations(t_moves *moves, t_block *block)
{
	flush_combined(moves, block);
	flush_single(moves, &block->write, block->rot_a, OP_RA);
	flush_single(moves, &block->write, block->rot_b, OP_RB);
	block->rot_a = 0;
	block->rot_b = 0;
}
