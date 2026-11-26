/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 14:03:10 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 17:56:06 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	init_moves(t_moves *moves);
int	add_move(t_moves *moves, t_op op);
int	grow_moves(t_moves *moves);
void	free_moves(t_moves *moves);

int	init_moves(t_moves *moves)
{
	if (!moves)
		return (1);
	moves->size = 0;
	moves->capacity = 0;
	moves->ops = malloc(sizeof(t_op) * 128);
	if (!moves->ops)
		return (1);
	moves->capacity = 128;
	return (0);
}

int	add_move(t_moves *moves, t_op op)
{
	if (!moves || !moves->ops)
		return (1);
	if (moves->size >= moves->capacity)
	{
		if (grow_moves(moves))
			return (1);
	}
	moves->ops[moves->size] = op;
	moves->size++;
	return (0);
}

int	grow_moves(t_moves *moves)
{
	int		i;
	t_op	*new_ops;
	int		new_capacity;

	if (!moves || !moves->ops)
		return (1);
	if (moves->capacity > INT_MAX / 2)
		return (1);
	i = 0;
	new_capacity = moves->capacity * 2;
	new_ops = malloc(sizeof(t_op) * new_capacity);
	if (!new_ops)
		return (1);
	while (i < moves->size)
	{
		new_ops[i] = moves->ops[i];
		i++;
	}
	free(moves->ops);
	moves->ops = new_ops;
	moves->capacity = new_capacity;
	return (0);
}

void	free_moves(t_moves *moves)
{
	if (!moves)
		return ;
	free(moves->ops);
	moves->ops = NULL;
	moves->size = 0;
	moves->capacity = 0;
}

/* int	main(void)
{
	t_moves	moves;
	int		i;

	if (init_moves(&moves))
		return (1);
	printf("Initial size: %d\n", moves.size);
	printf("Initial capacity: %d\n", moves.capacity);
	i = 0;
	while (i < 10000)
	{
		if (add_move(&moves, (t_op)(i % 11)))
		{
			free_moves(&moves);
			return (1);
		}
		i++;
	}
	printf("Final size: %d\n", moves.size);
	printf("Final capacity: %d\n", moves.capacity);
	i = 0;
	while (i < moves.size)
	{
		if (moves.ops[i] != (t_op)(i % 11))
		{
			printf("KO at index %d\n", i);
			free_moves(&moves);
			return (1);
		}
		i++;
	}
	printf("All operations preserved: OK\n");
	free_moves(&moves);
	if (!moves.ops && moves.size == 0 && moves.capacity == 0)
		printf("Free moves: OK\n");
	else
		return (1);
	return (0);
} */