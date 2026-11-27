/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 15:09:50 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 17:15:07 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	rotation_to_a(t_stack *a, t_stack *b, int size, int pivot);
static int	rotation_to_b(t_stack *a, t_stack *b, int size, int pivot);

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
	int	rotation;

	quant = 0;
	rotation = 0;
	while (quant < size / 2)
	{
		if (a->head->index < pivot)
		{
			pb(a, b);
			quant++;
		}
		else
		{
			ra(a);
			rotation++;
		}
	}
	return (rotation);
}

int	partition_a(t_stack *a, t_stack *b, int size, int pivot)
{
	int	full_stack;
	int	quant;
	int	rotation;

	if (!a || !b || size <= 0 || size > a->size)
		return (0);
	full_stack = (size == a->size);
	quant = size / 2;
	rotation = rotation_to_a(a, b, size, pivot);
	if (!full_stack)
	{
		while (rotation-- > 0)
			rra(a);
	}
	return (quant);
}

static int	rotation_to_b(t_stack *a, t_stack *b, int size, int pivot)
{
	int	quant;
	int	rotation;

	quant = 0;
	rotation = 0;
	while (quant < size - size / 2)
	{
		if (b->head->index >= pivot)
		{
			pa(a, b);
			quant++;
		}
		else
		{
			rb(b);
			rotation++;
		}
	}
	return (rotation);
}

int	partition_b(t_stack *a, t_stack *b, int size, int pivot)
{
	int	quant;
	int	rotation;
	int	full_stack;

	if (!a || !b || size <= 0 || size > b->size)
		return (0);
	full_stack = (size == b->size);
	quant = size - size / 2;
	rotation = rotation_to_b(a, b, size, pivot);
	if (!full_stack)
	{
		while (rotation-- > 0)
			rrb(b);
	}
	return (quant);
}
