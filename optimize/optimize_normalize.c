/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_normalize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:39:19 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 15:42:52 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	normalize_rotation(int rotation, int size)
{
	if (size < 2)
		return (0);
	rotation = rotation % size;
	if (rotation > size / 2)
		rotation -= size;
	else if (rotation < -(size / 2))
		rotation += size;
	return (rotation);
}

static int	rotation_cost(int rot_a, int rot_b)
{
	int	same_sign;

	same_sign = ((rot_a > 0 && rot_b > 0)
			|| (rot_a < 0 && rot_b < 0));
	if (rot_a < 0)
		rot_a = -rot_a;
	if (rot_b < 0)
		rot_b = -rot_b;
	if (same_sign)
	{
		if (rot_a > rot_b)
			return (rot_a);
		return (rot_b);
	}
	return (rot_a + rot_b);
}

void	normalize_block(t_block *block)
{
	int	new_a;
	int	new_b;

	new_a = normalize_rotation(block->rot_a, block->size_a);
	new_b = normalize_rotation(block->rot_b, block->size_b);
	if (rotation_cost(block->rot_a, block->rot_b)
		<= rotation_cost(new_a, new_b))
		return ;
	block->rot_a = new_a;
	block->rot_b = new_b;
}
