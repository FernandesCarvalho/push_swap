/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 11:57:20 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 13:40:32 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	find_max_pos(t_stack *b)
{
	int		pos;
	int		val;
	int		i;
	t_node	*current;

	if (!b || !b->head)
		return (0);
	current = b->head;
	i = 0;
	val = current->index;
	pos = 0;
	while (current)
	{
		if (current->index > val)
		{
			val = current->index;
			pos = i;
		}
		current = current->next;
		i++;
	}
	return (pos);
}

void	push_max_to_a(t_stack *a, t_stack *b)
{
	int	pos;
	int	i;

	i = 0;
	if (!a || !b || b->size == 0)
		return ;
	pos = find_max_pos(b);
	if (pos <= b->size / 2)
	{
		while (i++ < pos)
			rb(b);
	}
	else
	{
		i = b->size;
		while (i-- > pos)
			rrb(b);
	}
	pa(a, b);
}

void	push_chunk_node(t_stack *a, t_stack *b, int *pshed, t_chunk *chnk)
{
	int	index;

	if (!a || !a->head || !b || !chnk)
		return ;
	index = a->head->index;
	if (index >= chnk->start && index < chnk->end)
	{
		pb(a, b);
		if (index < chnk->mid)
			rb(b);
		(*pshed)++;
	}
	else
		ra(a);
}

static void	next_chunk(t_chunk *chunk, int *target, int chunk_size, int total)
{
	chunk->start = chunk->end;
	*target = total - chunk->start;
	if (*target > chunk_size)
		*target = chunk_size;
	chunk->end = chunk->start + *target;
	chunk->mid = chunk->start + *target / 2;
}

void	push_chunks_to_b(t_stack *a, t_stack *b)
{
	int		chunk_size;
	int		pushed;
	int		target;
	int		total;
	t_chunk	chunk;

	if (!a || !b || a->size == 0)
		return ;
	total = a->size;
	chunk_size = ft_sqrt(total);
	pushed = 0;
	target = chunk_size;
	chunk.start = 0;
	chunk.end = target;
	chunk.mid = target / 2;
	while (a->size > 0)
	{
		push_chunk_node(a, b, &pushed, &chunk);
		if (pushed == target)
		{
			next_chunk(&chunk, &target, chunk_size, total);
			pushed = 0;
		}
	}
}
