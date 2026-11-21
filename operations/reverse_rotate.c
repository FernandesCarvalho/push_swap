/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:00:44 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/21 10:46:45 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	reverse_rotate(t_stack *stack)
{
	t_node	*temp;

	if (!stack->head || !stack->head->next)
		return ;
	temp = stack->tail;
	stack->tail = temp->prev;
	stack->tail->next = NULL;
	temp->next = stack->head;
	temp->prev = NULL;
	stack->head->prev = temp;
	stack->head = temp;
}

void	rra(t_stack *a)
{
	if (a->size < 2)
		return ;
	reverse_rotate(a);
	write(1, "rra\n", 4);
}

void	rrb(t_stack *b)
{
	if (b->size < 2)
		return ;
	reverse_rotate(b);
	write(1, "rrb\n", 4);
}

void	rrr(t_stack *a, t_stack *b)
{
	if (a->size < 2 && b->size < 2)
		return ;
	reverse_rotate(a);
	reverse_rotate(b);
	write(1, "rrr\n", 4);
}