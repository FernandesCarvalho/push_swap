/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_jeanlezy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/24 00:47:30 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	init_stack(t_stack *stack)
{
	if (!stack)
		return ;
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
}

int	main(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;

	if (argc == 1)
		return (0);
	init_stack(&a);
	init_stack(&b);
	if (parse_args(argc, argv, &a, &b))
		return (1);
	if (is_sorted(&a))
	{
		free_stack(&a);
		free_stack(&b);
		return (0);
	}
	if (a.size == 2)
		sort_two(&a);
	else if (a.size == 3)
		sort_three(&a);
	else if (a.size == 4)
		sort_four(&a, &b);
	else if (a.size == 5)
		sort_five(&a, &b);
	else if (a.size > 5)
		sort_simple(&a, &b);
	free_stack(&a);
	free_stack(&b);
	return (0);
}