/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:04:52 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/27 11:40:25 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static void	put_pct_err(double disorder)
{
	long	scaled;
	long	whole;
	long	frac;

	if (disorder < 0.0)
		disorder = 0.0;
	scaled = (long)(disorder * 10000.0 + 0.5);
	whole = scaled / 100;
	frac = scaled % 100;
	put_nbr_err(whole);
	write(2, ".", 1);
	if (frac < 10)
		write(2, "0", 1);
	put_nbr_err(frac);
	write(2, "%\n", 2);
}

static void	put_strat_err(t_config *cfg, t_strategy used_strat)
{
	put_str_err("[bench] strategy: ");
	if (cfg->strategy == STRAT_ADAPTIVE)
		put_str_err("Adaptive / ");
	if (used_strat == STRAT_SIMPLE)
		put_str_err("Simple / O(n²)\n");
	else if (used_strat == STRAT_MEDIUM)
		put_str_err("Medium / O(n√n)\n");
	else
		put_str_err("Complex / O(n log n)\n");
}

static void	put_basic_err(t_ops *ops)
{
	put_str_err("[bench] total_ops: ");
	put_nbr_err(ops->total);
	put_str_err("\n[bench] sa: ");
	put_nbr_err(ops->sa);
	put_str_err(" sb: ");
	put_nbr_err(ops->sb);
	put_str_err(" ss: ");
	put_nbr_err(ops->ss);
	put_str_err(" pa: ");
	put_nbr_err(ops->pa);
	put_str_err(" pb: ");
	put_nbr_err(ops->pb);
	write(2, "\n", 1);
}

static void	put_rotate_err(t_ops *ops)
{
	put_str_err("[bench] ra: ");
	put_nbr_err(ops->ra);
	put_str_err(" rb: ");
	put_nbr_err(ops->rb);
	put_str_err(" rr: ");
	put_nbr_err(ops->rr);
	put_str_err(" rra: ");
	put_nbr_err(ops->rra);
	put_str_err(" rrb: ");
	put_nbr_err(ops->rrb);
	put_str_err(" rrr: ");
	put_nbr_err(ops->rrr);
	write(2, "\n", 1);
}

void	print_bench(t_config *cfg, t_strategy used_strat, double disorder)
{
	if (!cfg || !cfg->bench)
		return ;
	put_str_err("[bench] disorder: ");
	put_pct_err(disorder);
	put_strat_err(cfg, used_strat);
	put_basic_err(get_ops_stats());
	put_rotate_err(get_ops_stats());
}
