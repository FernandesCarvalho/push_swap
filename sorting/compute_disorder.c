/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_disorder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 10:27:11 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 12:50:17 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

double	compute_disorder(t_stack *a)
{
	t_node	*curr;
	t_node	*next;
	int		inversions;
	double	max_inversions;
	
	if (!a || a-> size < 2)
		return (0.0);
	inversions = 0;
	curr = a->head;
	while (curr)
	{
		next = curr->next;
		while (next)
		{
			if(curr->value > next->value)
				inversions++;
			next = next->next;
		}
		curr = curr->next;
	}
	max_inversions = (double)a->size * (a->size - 1) / 2.0;
	return ((double)inversions / max_inversions);
}