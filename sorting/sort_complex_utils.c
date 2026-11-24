/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 15:09:50 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/24 17:54:50 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	get_pivot(t_stack *stack, int size)
{
	t_node	*current;
	int		min_index;
	int		i;

	if (!stack || size <= 0 || size > stack->size)
		return (0);
	i = size;
	current = stack->head;
	min_index = current->index;
	while (i > 0)
	{
		if (current->index < min_index)
			min_index = current->index;
		current = current->next;
		i--;
	}
	return (min_index + size / 2);
}

static int	rotation_to_a(t_stack *a, t_stack *b, int size, int pivot)
{
	int	quant;

	quant = 0;
	while (size > 0)
	{
		if (a->head->index < pivot)
		{
			pb(a, b);
			quant++;
		}
		else
			ra(a);
		size--;
	}
	return (quant);
}

int	partition_a(t_stack *a, t_stack *b, int size, int pivot)
{
	int	full_stack;
	int	quant;
	int	rotation;

	if (!a || !b || size <= 0 || size > a->size)
		return (0);
	full_stack = (size == a->size);
	quant = rotation_to_a(a, b, size, pivot);
	rotation = size - quant;
	if (!full_stack)
		while (rotation-- > 0)
			rra(a);
	return (quant);
}

static int	rotation_to_b(t_stack *a, t_stack *b, int size, int pivot)
{
	int	quant;

	quant = 0;
	while (size > 0)
	{
		if (b->head->index >= pivot)
		{
			pa(a, b);
			quant++;
		}
		else
			rb(b);
		size--;
	}
	return (quant);
}

int	partition_b(t_stack *a, t_stack *b, int size, int pivot)
{
	int	quant;
	int	rotation;
	int	full_stack;

	if (!a || !b || size <= 0 || size > b->size)
		return (0);
	full_stack = (size == b->size);
	quant = rotation_to_b(a, b, size, pivot);
	rotation = size - quant;

	if (!full_stack)
		while (rotation-- > 0)
			rrb(b);
	return (quant);
}
