/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_final.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 03:14:13 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:37:26 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	optimize_moves(t_moves *moves, int initial_size)
{
	int	before;

	if (!moves || !moves->ops || moves->error)
		return ;
	while (1)
	{
		before = moves->size;
		optimize_pairs(moves);
		optimize_blocks(moves, initial_size, 0);
		optimize_pairs(moves);
		optimize_merge(moves, initial_size);
		optimize_pairs(moves);
		if (before == moves->size)
			return ;
	}
}
