/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_group.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:37:53 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 13:46:55 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int		is_group_sorted(t_stack *stack, int size);
void	sort_three_group(t_stack *a);

int	is_group_sorted(t_stack *stack, int size)
{
	t_node	*current;
	int		cur_ind;
	int		next_ind;

	if (!stack || !stack->head || size <= 0 || size > stack->size)
		return (0);
	current = stack->head;
	while (size > 1)
	{
		cur_ind = current->index;
		next_ind = current->next->index;
		if (cur_ind > next_ind)
			return (0);
		size--;
		current = current->next;
	}
	return (1);
}

void	sort_three_group(t_stack *a)
{
	if (!a || !a->head || a->size < 3)
		return ;
	if (a->size == 3)
	{
		sort_three(a);
		return ;
	}
	if (a->head->index > a->head->next->index)
		sa(a);
	if (a->head->next->index > a->head->next->next->index)
	{
		ra(a);
		sa(a);
		rra(a);
	}
	if (a->head->index > a->head->next->index)
		sa(a);
}

void	sort_three_group_b(t_stack *a, t_stack *b)
{
	if (!a || !b || b->size < 3)
		return ;
	if (b->head->next->index > b->head->index
		&& b->head->next->index > b->head->next->next->index)
		sb(b);
	if (b->head->index > b->head->next->next->index)
	{
		pa(a, b);
		if (b->head->index < b->head->next->index)
			sb(b);
		pa(a, b);
		pa(a, b);
		return ;
	}
	if (b->head->index < b->head->next->index)
		sb(b);
	pa(a, b);
	sb(b);
	pa(a, b);
	sa(a);
	pa(a, b);
}
