/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 12:15:10 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:15:03 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

#include "push_swap.h"

static void	split_operation(t_op op, t_moves *a, t_moves *b);
static int	can_merge(t_op op_a, t_op op_b);
static void	merge_operations(t_moves *a, t_moves *b,
	t_moves *result);
static void	normalize_sequence(t_moves *sequence,
	int stack_size, t_op forward);
static void	process_block(t_merge *merge, int size_a, int size_b);
static void	write_block(t_moves *moves, t_merge *merge,
	int *write, int start, int end);

static void	split_operation(t_op op, t_moves *a, t_moves *b)
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

static void	merge_operations(t_moves *a, t_moves *b,
	t_moves *result)
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
		else if (i + 1 < a->size
			&& can_merge(a->ops[i + 1], b->ops[j]))
		{
			add_move(result, a->ops[i]);
			i++;
		}
		else if (j + 1 < b->size
			&& can_merge(a->ops[i], b->ops[j + 1]))
		{
			add_move(result, b->ops[j]);
			j++;
		}
		else
		{
			add_move(result, a->ops[i]);
			i++;
		}
	}
	while (i < a->size)
	{
		add_move(result, a->ops[i]);
		i++;
	}
	while (j < b->size)
	{
		add_move(result, b->ops[j]);
		j++;
	}
}

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

static void	process_block(t_merge *merge, int size_a, int size_b)
{
	merge->result.size = 0;
	normalize_sequence(&merge->a, size_a, OP_RA);
	normalize_sequence(&merge->b, size_b, OP_RB);
	merge_operations(&merge->a, &merge->b, &merge->result);
}

void	optimize_merge(t_moves *moves, int initial_size)
{
	t_merge	merge;
	int		read;
	int		write;
	int		size_a;
	int		size_b;
	int		start;

	if (!moves || !moves->ops || moves->error)
		return ;
	if (init_moves(&merge.a))
		return ;
	if (init_moves(&merge.b))
	{
		free_moves(&merge.a);
		return ;
	}
	if (init_moves(&merge.result))
	{
		free_moves(&merge.a);
		free_moves(&merge.b);
		return ;
	}
	read = 0;
	write = 0;
	start = 0;
	size_a = initial_size;
	size_b = 0;
	while (read < moves->size)
	{
		if (moves->ops[read] != OP_PA && moves->ops[read] != OP_PB)
			split_operation(moves->ops[read], &merge.a, &merge.b);
		else
		{
			process_block(&merge, size_a, size_b);
			write_block(moves, &merge, &write, start, read);
			if (moves->ops[read] == OP_PA)
			{
				size_a++;
				size_b--;
			}
			else
			{
				size_a--;
				size_b++;
			}
			moves->ops[write] = moves->ops[read];
			write++;
			merge.a.size = 0;
			merge.b.size = 0;
			start = read + 1;
		}
		read++;
	}
	process_block(&merge, size_a, size_b);
	write_block(moves, &merge, &write, start, read);
	moves->size = write;
	free_moves(&merge.a);
	free_moves(&merge.b);
	free_moves(&merge.result);
}

static void	write_block(t_moves *moves, t_merge *merge,
	int *write, int start, int end)
{
	int	i;

	if (!merge->a.error && !merge->b.error
		&& !merge->result.error
		&& merge->result.size < end - start)
	{
		i = 0;
		while (i < merge->result.size)
		{
			moves->ops[*write] = merge->result.ops[i];
			(*write)++;
			i++;
		}
	}
	else
	{
		while (start < end)
		{
			moves->ops[*write] = moves->ops[start];
			(*write)++;
			start++;
		}
	}
}
