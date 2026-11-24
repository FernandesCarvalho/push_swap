/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:20:58 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/24 11:01:36 by jeanbarr         ###   ########.fr       */
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
	write(1, "ra\n", 3);
}

void	rb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	rotate(b);
	write(1, "rb\n", 3);
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
		write(1, "rr\n", 3);
}
