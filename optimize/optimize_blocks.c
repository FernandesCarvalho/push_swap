/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_blocks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 14:02:47 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 14:26:06 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void		optimize_blocks(t_moves *moves, int size_a, int size_b);
static int	is_rotation(t_op op);
static void	count_rotation(t_op op, int *rot_a, int *rot_b);
static int	normalize_rotation(int rotation, int size);
static int	rotation_cost(int rot_a, int rot_b);
static void	normalize_block(int *rot_a, int *rot_b,
	int size_a, int size_b);
static void	flush_rotations(t_moves *moves, int *write,
	int rot_a, int rot_b);


static int	is_rotation(t_op op)
{
	return (op >= OP_RA && op <= OP_RRR);
}
static void	count_rotation(t_op op, int *rot_a, int *rot_b)
{
	if (op == OP_RA || op == OP_RR)
		(*rot_a)++;
	if (op == OP_RB || op == OP_RR)
		(*rot_b)++;
	if (op == OP_RRA || op == OP_RRR)
		(*rot_a)--;
	if (op == OP_RRB || op == OP_RRR)
		(*rot_b)--;
}

static void	flush_rotations(t_moves *moves, int *write,
	int rot_a, int rot_b)
{
	while (rot_a > 0 && rot_b > 0)
	{
		moves->ops[*write] = OP_RR;
		(*write)++;
		rot_a--;
		rot_b--;
	}
	while (rot_a < 0 && rot_b < 0)
	{
		moves->ops[*write] = OP_RRR;
		(*write)++;
		rot_a++;
		rot_b++;
	}
	while (rot_a > 0)
	{
		moves->ops[*write] = OP_RA;
		(*write)++;
		rot_a--;
	}
	while (rot_b > 0)
	{
		moves->ops[*write] = OP_RB;
		(*write)++;
		rot_b--;
	}
	while (rot_a < 0)
	{
		moves->ops[*write] = OP_RRA;
		(*write)++;
		rot_a++;
	}
	while (rot_b < 0)
	{
		moves->ops[*write] = OP_RRB;
		(*write)++;
		rot_b++;
	}	
}

void	optimize_blocks(t_moves *moves, int size_a, int size_b)
{
	int	read;
	int	write;
	int	rot_a;
	int	rot_b;

	if (!moves || !moves->ops || moves->error)
		return ;
	read = 0;
	write = 0;
	rot_a = 0;
	rot_b = 0;
	while (read < moves->size)
	{
		if (is_rotation(moves->ops[read]))
			count_rotation(moves->ops[read], &rot_a, &rot_b);
		else
		{
			normalize_block(&rot_a, &rot_b, size_a, size_b);
			flush_rotations(moves, &write, rot_a, rot_b);
			if (moves->ops[read] == OP_PA)
			{
				size_a++;
				size_b--;
			}
			else if (moves->ops[read] == OP_PB)
			{
				size_a--;
				size_b++;
			}
			moves->ops[write] = moves->ops[read];
			write++;
			rot_a = 0;
			rot_b = 0;
		}
		read++;
	}
	normalize_block(&rot_a, &rot_b, size_a, size_b);
	flush_rotations(moves, &write, rot_a, rot_b);
	moves->size = write;
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

static void	normalize_block(int *rot_a, int *rot_b,
	int size_a, int size_b)
{
	int	new_a;
	int	new_b;

	new_a = normalize_rotation(*rot_a, size_a);
	new_b = normalize_rotation(*rot_b, size_b);
	if (rotation_cost(*rot_a, *rot_b) <= rotation_cost(new_a, new_b))
		return ;
	*rot_a = new_a;
	*rot_b = new_b;
}

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