/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_jeanlezy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/26 17:59:34 by fercarva         ###   ########.fr       */
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
	stack->moves = NULL;
}

static void	dispatch_sort(t_stack *a, t_stack *b, t_config *config)
{
	if (a->size == 2)
		sort_two(a);
	else if (a->size == 3)
		sort_three(a);
	else if (a->size == 4)
		sort_four(a, b);
	else if (a->size == 5)
		sort_five(a, b);
	else if (config->strategy == STRAT_SIMPLE)
		sort_simple(a, b);
	else if (config->strategy == STRAT_MEDIUM)
		sort_medium(a, b);
	else if (config->strategy == STRAT_COMPLEX)
		sort_complex(a, b);
	else
		sort_medium(a, b);
}

int	main(int argc, char **argv)
{
	t_stack		a;
	t_stack		b;
	t_config	config;
	t_moves		moves;

	if (argc == 1)
		return (0);
	parse_flags(argc, argv, &config);
	if (config.arg_start >= argc)
		return (0);
	init_stack(&a);
	init_stack(&b);
	if (parse_args(argc, argv, &a, &b, config.arg_start))
		return (1);
	if (init_moves(&moves))
		return (error_exit(&a, &b, NULL));
	a.moves = &moves;
	b.moves = &moves;
	if (!is_sorted(&a))
		dispatch_sort(&a, &b, &config);
		
	if (is_sorted(&a) && b.size == 0)
		write(2, "OK\n", 3);
	else
		write(2, "KO\n", 3);
	free_moves(&moves);
	free_stack(&a);
	free_stack(&b);
	return (0);
}