/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 20:46:02 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 13:43:13 by jeanbarr         ###   ########.fr       */
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
}

int	main(int argc, char **argv)
{
	t_stack		a;
	t_stack		b;
	t_config	config;
	double		disorder;

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
	if (config.strategy == STRAT_ADAPTIVE)
	{
		disorder = compute_disorder(&a);
		config.strategy = choose_adaptive(disorder);
	}
	if (!is_sorted(&a))
		dispatch_sort(&a, &b, &config);
	free_stack(&a);
	free_stack(&b);
	return (0);
}
