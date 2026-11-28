/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:20:58 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 18:49:17 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	rotate(t_stack *stack)
{
	t_node	*temp;

	if (!stack || !stack->head || !stack->head->next || !stack->tail)
		return ;
	temp = stack->head;
	stack->head = temp->next;
	stack->head->prev = NULL;
	stack->tail->next = temp;
	temp->next = NULL;
	temp->prev = stack->tail;
	stack->tail = temp;
}

void	ra(t_stack *a)
{
	if (!a || a->size < 2)
		return ;
	rotate(a);
	add_move(a->moves, OP_RA);
}

void	rb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	rotate(b);
	add_move(b->moves, OP_RB);
}

void	rr(t_stack *a, t_stack *b)
{
	int	did_rotate;

	did_rotate = 0;
	if (a && a->size >= 2)
	{
		rotate(a);
		did_rotate = 1;
	}
	if (b && b->size >= 2)
	{
		rotate(b);
		did_rotate = 1;
	}
	if (did_rotate)
	{
		if (a)
			add_move(a->moves, OP_RR);
		else
			add_move(b->moves, OP_RR);
	}
}
