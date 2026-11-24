/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 17:57:31 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/24 19:14:08 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	sort_a(t_stack *a, t_stack *b, int size)
{
	int	pivot;
	int	pushed;

	if (!a || !b || size <= 1 || size > a->size)
		return ;
	if (size == 2)
	{
		if (a->head->index > a->head->next->index)
			sa(a);
		return ;
	}
	pivot = get_pivot(a, size);
	pushed = partition_a(a, b, size, pivot);
	sort_a(a, b, size - pushed);
}

static void	sort_b(t_stack *a, t_stack *b, int size)
{
	int	pivot;
	int	pushed;

	if (!a || !b || size <= 0 || size > b->size)
		return ;
	if (size == 1)
	{
		pa(a, b);
		return ;
	}
	if (size == 2)
	{
		if (b->head->index < b->head->next->index)
			sb(b);
		pa(a, b);
		pa(a, b);
		return ;
	}
	pivot = get_pivot(b, size);
	pushed = partition_b(a, b, size, pivot);
	sort_a(a, b, pushed);
	sort_b(a, b, size - pushed);
}

void		sort_complex(t_stack *a, t_stack *b)
{
	
}