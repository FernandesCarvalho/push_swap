/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 16:28:58 by fercarva         ###   ########.fr       */
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

static int	sort_tiny(t_stack *a, t_stack *b)
{
	if (a->size == 2)
		sort_two(a);
	else if (a->size == 3)
		sort_three(a);
	else if (a->size == 4)
		sort_four(a, b);
	else if (a->size == 5)
		sort_five(a, b);
	else
		return (0);
	return (1);
}

static void	dispatch_sort(t_stack *a, t_stack *b, t_strategy used_strat)
{
	if (sort_tiny(a, b))
		return ;
	if (used_strat == STRAT_SIMPLE)
		sort_simple(a, b);
	else if (used_strat == STRAT_MEDIUM)
		sort_medium(a, b);
	else
		sort_complex(a, b);
}

static t_strategy	resolve_strategy(t_config *config, double disorder)
{
	if (config->strategy == STRAT_ADAPTIVE)
		return (choose_adaptive(disorder));
	return (config->strategy);
}

int	main(int argc, char **argv)
{
	t_stack		a;
	t_stack		b;
	t_config	config;
	t_strategy	used_strat;
	double		disorder;
	t_moves		moves;
	int			initial_size;

	if (argc == 1)
		return (0);
	parse_flags(argc, argv, &config);
	if (config.arg_start >= argc)
		return (0);
	init_stack(&a);
	init_stack(&b);
	if (parse_args(argc - config.arg_start,
			argv + config.arg_start, &a, &b))
		return (1);
	initial_size = a.size;
	disorder = compute_disorder(&a);
	used_strat = resolve_strategy(&config, disorder);
	if (init_moves(&moves))
		return (error_exit(&a, &b, NULL));
	a.moves = &moves;
	b.moves = &moves;
	if (!is_sorted(&a))
		dispatch_sort(&a, &b, used_strat);
	if (moves.error)
	{
		free_moves(&moves);
		return (error_exit(&a, &b, NULL));
	}
	optimize_moves(&moves, initial_size);
	if (config.bench)
		count_moves(&moves);
	print_moves(&moves);
	print_bench(&config, used_strat, disorder);
	free_moves(&moves);
	free_stack(&a);
	free_stack(&b);
	return (0);
}
