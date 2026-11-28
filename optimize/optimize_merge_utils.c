/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_merge_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:49:34 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:49:41 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	normalize_sequence(t_moves *sequence,
	int stack_size, t_op forward)
{
	if (stack_size < 2)
	{
		sequence->size = 0;
		return ;
	}
	optimize_pairs(sequence);
	if (forward == OP_RA)
		optimize_blocks(sequence, stack_size, 0);
	else
		optimize_blocks(sequence, 0, stack_size);
	optimize_pairs(sequence);
}

void	process_block(t_merge *merge)
{
	merge->result.size = 0;
	normalize_sequence(&merge->a, merge->size_a, OP_RA);
	normalize_sequence(&merge->b, merge->size_b, OP_RB);
	merge_operations(&merge->a, &merge->b, &merge->result);
}

static int	is_better_block(t_merge *merge)
{
	if (merge->a.error || merge->b.error || merge->result.error)
		return (0);
	return (merge->result.size < merge->read - merge->start);
}

void	write_block(t_moves *moves, t_merge *merge)
{
	int	i;

	if (is_better_block(merge))
	{
		i = 0;
		while (i < merge->result.size)
		{
			moves->ops[merge->write] = merge->result.ops[i];
			merge->write++;
			i++;
		}
	}
	else
	{
		i = merge->start;
		while (i < merge->read)
		{
			moves->ops[merge->write] = moves->ops[i];
			merge->write++;
			i++;
		}
	}
}
