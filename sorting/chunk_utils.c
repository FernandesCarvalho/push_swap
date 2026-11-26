/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 11:57:20 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/26 12:10:04 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	chunk_rotation(t_stack *a, t_chunk *chnk)
{
	t_node	*curr;
	int		i;
	int		first;
	int		last;

	curr = a->head;
	i = 0;
	first = -1;
	last = -1;
	while (curr)
	{
		if (curr->index >= chnk->start && curr->index < chnk->end)
		{
			if (first == -1)
				first = i;
			last = i;
		}
		curr = curr->next;
		i++;
	}
	if (first <= a->size - last)
		return (first);
	return (last - a->size);
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
		return ;
	}
	if (chunk_rotation(a, chnk) >= 0)
		ra(a);
	else
		rra(a);
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
	chunk_size = ft_sqrt(total) * 2;
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
