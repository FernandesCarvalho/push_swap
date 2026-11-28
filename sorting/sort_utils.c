/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:35:37 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 10:57:52 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	assign_index(t_stack *a)
{
	int		index;
	t_node	*check;
	t_node	*current;

	if (!a || a->size < 1)
		return ;
	current = a->head;
	while (current)
	{
		check = a->head;
		index = 0;
		while (check)
		{
			if (current->value > check->value)
				index++;
			check = check->next;
		}
		current->index = index;
		current = current->next;
	}
}

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

int	find_min_pos(t_stack *a)
{
	int		pos;
	int		val;
	int		i;
	t_node	*current;

	if (!a || !a->head)
		return (0);
	current = a->head;
	i = 0;
	val = current->value;
	pos = 0;
	while (current)
	{
		if (current->value < val)
		{
			val = current->value;
			pos = i;
		}
		current = current->next;
		i++;
	}
	return (pos);
}

void	push_min_to_b(t_stack *a, t_stack *b)
{
	int	pos;

	if (!a || !b || a->size == 0)
		return ;
	pos = find_min_pos(a);
	if (pos <= a->size / 2)
	{
		while (pos-- > 0)
			ra(a);
	}
	else
	{
		while (pos++ < a->size)
			rra(a);
	}
	pb(a, b);
}

int	ft_sqrt(int n)
{
	int	i;

	if (n < 1)
		return (0);
	i = 1;
	while (i <= n / i)
		i++;
	return (i - 1);
}
