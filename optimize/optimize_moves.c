/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_moves.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 14:03:10 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:31:21 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	init_moves(t_moves *moves)
{
	if (!moves)
		return (1);
	moves->size = 0;
	moves->capacity = 0;
	moves->error = 0;
	moves->ops = malloc(sizeof(t_op) * 128);
	if (!moves->ops)
		return (1);
	moves->capacity = 128;
	return (0);
}

int	add_move(t_moves *moves, t_op op)
{
	if (!moves || !moves->ops || moves->error)
		return (1);
	if (moves->size >= moves->capacity)
	{
		if (grow_moves(moves))
		{
			moves->error = 1;
			return (1);
		}
	}
	moves->ops[moves->size] = op;
	moves->size++;
	return (0);
}

int	grow_moves(t_moves *moves)
{
	int		i;
	t_op	*new_ops;
	int		new_capacity;

	if (!moves || !moves->ops)
		return (1);
	if (moves->capacity > INT_MAX / 2)
		return (1);
	i = 0;
	new_capacity = moves->capacity * 2;
	new_ops = malloc(sizeof(t_op) * new_capacity);
	if (!new_ops)
		return (1);
	while (i < moves->size)
	{
		new_ops[i] = moves->ops[i];
		i++;
	}
	free(moves->ops);
	moves->ops = new_ops;
	moves->capacity = new_capacity;
	return (0);
}

void	print_moves(t_moves *moves)
{
	static const char	*op[] = {
		"sa\n", "sb\n", "ss\n", "pa\n", "pb\n",
		"ra\n", "rb\n", "rr\n", "rra\n", "rrb\n", "rrr\n"
	};
	int					i;

	if (!moves || !moves->ops || moves->error)
		return ;
	i = 0;
	while (i < moves->size)
	{
		if (moves->ops[i] >= OP_RRA)
			write(1, op[moves->ops[i]], 4);
		else
			write(1, op[moves->ops[i]], 3);
		i++;
	}
}

void	free_moves(t_moves *moves)
{
	if (!moves)
		return ;
	free(moves->ops);
	moves->ops = NULL;
	moves->size = 0;
	moves->capacity = 0;
	moves->error = 0;
}
