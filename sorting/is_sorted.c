/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_sorted.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:35:37 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/22 20:38:47 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

// Estão em ordem crescente do topo (head) até o fundo (tail)
int	is_sorted(t_stack *stack)
{
	t_node	*curr;

	if (!stack || stack->size <= 1)
		return (1);
	curr = stack->head;
	while (curr && curr->next)
	{
		if (curr->value > curr->next->value)
			return (0);
		curr = curr->next;
	}
	return (1);
}