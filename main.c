/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 17:22:03 by fercarva         ###   ########.fr       */
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

static int	run_sort(t_program *prog)
{
	prog->initial_size = prog->a.size;
	prog->disorder = compute_disorder(&prog->a);
	prog->used_strat = prog->config.strategy;
	if (prog->used_strat == STRAT_ADAPTIVE)
		prog->used_strat = choose_adaptive(prog->disorder);
	if (init_moves(&prog->moves))
		return (error_exit(&prog->a, &prog->b, NULL));
	prog->a.moves = &prog->moves;
	prog->b.moves = &prog->moves;
	if (!is_sorted(&prog->a))
		dispatch_sort(&prog->a, &prog->b, prog->used_strat);
	if (prog->moves.error)
	{
		free_moves(&prog->moves);
		return (error_exit(&prog->a, &prog->b, NULL));
	}
	optimize_moves(&prog->moves, prog->initial_size);
	if (prog->config.bench)
		count_moves(&prog->moves);
	print_moves(&prog->moves);
	print_bench(&prog->config, prog->used_strat, prog->disorder);
	free_moves(&prog->moves);
	return (0);
}

int	main(int argc, char **argv)
{
	t_program	prog;

	if (argc == 1)
		return (0);
	parse_flags(argc, argv, &prog.config);
	if (prog.config.arg_start >= argc)
		return (0);
	init_stack(&prog.a);
	init_stack(&prog.b);
	if (parse_args(argc - prog.config.arg_start,
			argv + prog.config.arg_start, &prog.a, &prog.b))
		return (1);
	if (run_sort(&prog))
		return (1);
	free_stack(&prog.a);
	free_stack(&prog.b);
	return (0);
}
