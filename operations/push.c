/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:00:13 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 18:53:12 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	push(t_stack *dst, t_stack *src)
{
	t_node	*temp;

	if (!dst || !src || !src->head)
		return ;
	temp = src->head;
	src->head = temp->next;
	if (src->head)
		src->head->prev = NULL;
	else
		src->tail = NULL;
	src->size--;
	if (dst->head)
	{
		temp->next = dst->head;
		dst->head->prev = temp;
	}
	else
	{
		dst->tail = temp;
		temp->next = NULL;
	}
	dst->head = temp;
	temp->prev = NULL;
	dst->size++;
}

void	pa(t_stack *a, t_stack *b)
{
	if (!a || !b || !b->head)
		return ;
	push(a, b);
	add_move(a->moves, OP_PA);
}

void	pb(t_stack *a, t_stack *b)
{
	if (!a || !b || !a->head)
		return ;
	push(b, a);
	add_move(b->moves, OP_PB);
}
