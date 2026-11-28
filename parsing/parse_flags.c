/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 18:03:38 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 13:52:31 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	init_config(t_config *config)
{
	config->strategy = STRAT_ADAPTIVE;
	config->bench = 0;
	config->arg_start = 1;
}

static int	same_str(const char *s1, const char *s2)
{
	size_t	len;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen(s2);
	return (ft_strncmp(s1, s2, len + 1) == 0);
}

int	set_bench(t_config *config, char *arg)
{
	if (same_str(arg, "--bench"))
	{
		config->bench = 1;
		return (1);
	}
	return (0);
}

int	set_strategy(t_config *config, char *arg)
{
	if (same_str(arg, "--adaptive"))
		config->strategy = STRAT_ADAPTIVE;
	else if (same_str(arg, "--simple"))
		config->strategy = STRAT_SIMPLE;
	else if (same_str(arg, "--medium"))
		config->strategy = STRAT_MEDIUM;
	else if (same_str(arg, "--complex"))
		config->strategy = STRAT_COMPLEX;
	else
		return (0);
	return (1);
}

void	parse_flags(int argc, char **argv, t_config *config)
{
	int	i;

	if (!config || !argv)
		return ;
	init_config(config);
	i = 1;
	while (i < argc)
	{
		if (set_bench(config, argv[i])
			|| set_strategy(config, argv[i]))
			i++;
		else
			break ;
	}
	config->arg_start = i;
}
