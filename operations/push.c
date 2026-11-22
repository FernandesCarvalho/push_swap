/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:00:13 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/22 17:36:52 by jeanbarr         ###   ########.fr       */
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
	write(1, "pa\n", 3);
}

void	pb(t_stack *a, t_stack *b)
{
	if (!a || !b || !a->head)
		return ;
	push(b, a);
	write(1, "pb\n", 3);
}
