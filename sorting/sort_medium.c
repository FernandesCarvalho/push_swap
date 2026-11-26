/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 00:50:44 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 12:56:09 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	rot_b_to_idx(t_stack *b, int idx)
{
	int		pos;
	int		i;
	t_node	*curr;

	curr = b->head;
	pos = 0;
	while (curr && curr->index != idx)
	{
		curr = curr->next;
		pos++;
	}
	i = 0;
	if (pos <= b->size / 2)
		while (i++ < pos)
			rb(b);
	else
	{
		i = b->size;
		while (i-- > pos)
			rrb(b);
	}
}

static int	idx_dist(t_stack *b, int idx)
{
	int		pos;
	t_node	*curr;

	curr = b->head;
	pos = 0;
	while (curr && curr->index != idx)
	{
		curr = curr->next;
		pos++;
	}
	if (pos <= b->size / 2)
		return (pos);
	return (b->size - pos);
}

void	push_back_to_a(t_stack *a, t_stack *b)
{
	int	max;

	if (!a || !b)
		return ;
	while (b->size > 0)
	{
		max = b->size - 1;
		if (b->size > 1 && idx_dist(b, max - 1) < idx_dist(b, max))
		{
			rot_b_to_idx(b, max - 1);
			pa(a, b);
			rot_b_to_idx(b, max);
			pa(a, b);
			sa(a);
		}
		else
		{
			rot_b_to_idx(b, max);
			pa(a, b);
		}
	}
}

void	sort_medium(t_stack *a, t_stack *b)
{
	if (!a || !b || is_sorted(a))
		return ;
	assign_index(a);
	push_chunks_to_b(a, b);
	push_back_to_a(a, b);
}
