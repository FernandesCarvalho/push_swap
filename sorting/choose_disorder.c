/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_disorder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 10:27:11 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 10:59:30 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

t_strategy	choose_adaptive(double disorder)
{
	if (disorder < 0.2)
		return (STRAT_SIMPLE);
	if (disorder < 0.5)
		return (STRAT_MEDIUM);
	return (STRAT_COMPLEX);
}

double	compute_disorder(t_stack *a)
{
	t_node	*curr;
	t_node	*next;
	int		inversions;
	double	max_inversions;

	if (!a || a->size < 2)
		return (0.0);
	inversions = 0;
	curr = a->head;
	while (curr)
	{
		next = curr->next;
		while (next)
		{
			if (curr->value > next->value)
				inversions++;
			next = next->next;
		}
		curr = curr->next;
	}
	max_inversions = (double)a->size * (a->size - 1) / 2.0;
	return ((double)inversions / max_inversions);
}
