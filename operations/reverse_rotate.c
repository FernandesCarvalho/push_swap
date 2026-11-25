/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:00:44 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/25 13:50:36 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	reverse_rotate(t_stack *stack)
{
	t_node	*temp;

	if (!stack || !stack->head || !stack->head->next || !stack->tail)
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
	if (!a || a->size < 2)
		return ;
	reverse_rotate(a);
	record_op(&get_ops_stats()->rra);
	write(1, "rra\n", 4);
}

void	rrb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	reverse_rotate(b);
	record_op(&get_ops_stats()->rrb);
	write(1, "rrb\n", 4);
}

void	rrr(t_stack *a, t_stack *b)
{
	int	did_rev_rot;

	did_rev_rot = 0;
	if (a && a->size >= 2)
	{
		reverse_rotate(a);
		did_rev_rot = 1;
	}
	if (b && b->size >= 2)
	{
		reverse_rotate(b);
		did_rev_rot = 1;
	}
	if (did_rev_rot)
	{
		record_op(&get_ops_stats()->rrr);
		write(1, "rrr\n", 4);
	}
}
