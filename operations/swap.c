/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:32:06 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/20 12:45:02 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*temp;

	if (!stack->head || !stack->head->next)
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
	swap(a);
	write(1, "sa\n", 3);
}

void	sb(t_stack *b)
{
	swap(b);
	write(1, "sb\n", 3);
}

void	ss(t_stack *a, t_stack *b)
{
	swap(a);
	swap(b);
	write(1, "ss\n", 3);
}
