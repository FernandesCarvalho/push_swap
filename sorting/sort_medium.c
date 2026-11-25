/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 00:50:44 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/24 17:26:01 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	push_back_to_a(t_stack *a, t_stack *b)
{
	if (!a || !b)
		return ;
	while (b->size > 0)
		push_max_to_a(a, b);
}

void	sort_medium(t_stack *a, t_stack *b)
{
	if (!a || !b || is_sorted(a))
		return ;
	assign_index(a);
	push_chunks_to_b(a, b);
	push_back_to_a(a, b);
}
