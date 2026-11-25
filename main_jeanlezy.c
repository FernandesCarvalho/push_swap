/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_jeanlezy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 08:58:02 by fercarva         ###   ########.fr       */
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
	if (!is_sorted(&a))
		sort_complex(&a, &b);
	if (is_sorted(&a) && b.size == 0)
		write(2, "OK\n", 3);
	else
		write(2, "KO\n", 3);
	free_stack(&a);
	free_stack(&b);
	return (0);
}