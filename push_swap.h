/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:37:33 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/20 14:16:09 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdio.h>

typedef struct s_node
{
	int	value;
	struct s_node	*prev;
	struct s_node	*next;

} t_node;

typedef struct s_stack
{
	t_node	*head;
	t_node	*tail;
	int		size;
} t_stack;

//                             ****    OPERATIONS     *****

//                    #      SWAP

void	swap(t_stack *stack);
void	sa(t_stack *a);
void	sb(t_stack *b);
void	ss(t_stack *a, t_stack *b);

//                    #      PUSH

void	push(t_stack *dst, t_stack *src);
void	pa(t_stack *a, t_stack *b);
void	pb(t_stack *a, t_stack *b);

//                             ****    STACK     *****

void	add_node_back(t_stack *stack, t_node *new_node);

#endif