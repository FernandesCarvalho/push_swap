/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 11:57:20 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 11:04:13 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	get_range(int total)
{
	int	range;

	range = (ft_sqrt(total) * 16) / 10;
	if (range < 1)
		range = 1;
	return (range);
}

static void	push_chunk_node(t_stack *a, t_stack *b, int *pshed, t_chunk *chnk)
{
	int	index;

	if (!a || !a->head || !b || !chnk)
		return ;
	index = a->head->index;
	if (index <= *pshed)
	{
		pb(a, b);
		if (a->head && a->head->index > *pshed + chnk->mid)
			rr(a, b);
		else
			rb(b);
		(*pshed)++;
	}
	else if (index <= *pshed + chnk->mid)
	{
		pb(a, b);
		(*pshed)++;
	}
	else
		ra(a);
}

void	push_chunks_to_b(t_stack *a, t_stack *b)
{
	int		pushed;
	t_chunk	chunk;

	if (!a || !b || a->size == 0)
		return ;
	pushed = 0;
	chunk.start = 0;
	chunk.end = a->size;
	chunk.mid = get_range(a->size);
	while (a->size > 0)
		push_chunk_node(a, b, &pushed, &chunk);
}
