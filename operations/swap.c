/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:32:06 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 18:48:26 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*temp;

	if (!stack || !stack->head || !stack->head->next)
		return ;
	first = stack->head;
	second = stack->head->next;
	temp = second->next;
	second->prev = NULL;
	second->next = first;
	first->next = temp;
	first->prev = second;
	stack->head = second;
	if (temp)
		temp->prev = first;
	else
		stack->tail = first;
}

void	sa(t_stack *a)
{
	if (!a || a->size < 2)
		return ;
	swap(a);
	add_move(a->moves, OP_SA);
}

void	sb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	swap(b);
	add_move(b->moves, OP_SB);
}

void	ss(t_stack *a, t_stack *b)
{
	int	did_swap;

	did_swap = 0;
	if (a && a->size >= 2)
	{
		swap(a);
		did_swap = 1;
	}
	if (b && b->size >= 2)
	{
		swap(b);
		did_swap = 1;
	}
	if (did_swap)
	{
		if (a)
			add_move(a->moves, OP_SS);
		else
			add_move(b->moves, OP_SS);
	}
}
