/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 23:26:23 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/26 13:32:29 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

static int	target_pos_a(t_stack *a, int v)
{
	t_node	*curr;
	int		i;
	int		pos;
	int		best;

	curr = a->head;
	i = 0;
	pos = -1;
	best = 0;
	while (curr)
	{
		if (curr->value > v && (pos == -1 || curr->value < best))
		{
			pos = i;
			best = curr->value;
		}
		curr = curr->next;
		i++;
	}
	if (pos == -1)
		return (find_min_pos(a));
	return (pos);
}

static void	eval_rot(int ca, int cb, int *mv)
{
	int	ax;
	int	ay;
	int	cost;

	ax = ca;
	if (ax < 0)
		ax = -ax;
	ay = cb;
	if (ay < 0)
		ay = -ay;
	if ((ca >= 0) != (cb >= 0))
		cost = ax + ay;
	else if (ax > ay)
		cost = ax;
	else
		cost = ay;
	if (cost < mv[2])
	{
		mv[0] = ca;
		mv[1] = cb;
		mv[2] = cost;
	}
}

static void	pick_cheapest(t_stack *a, t_stack *b, int *mv)
{
	t_node	*curr;
	int		j;
	int		p;

	mv[2] = INT_MAX;
	curr = b->head;
	j = 0;
	while (curr)
	{
		p = target_pos_a(a, curr->value);
		eval_rot(p, j, mv);
		eval_rot(p, j - b->size, mv);
		eval_rot(p - a->size, j, mv);
		eval_rot(p - a->size, j - b->size, mv);
		curr = curr->next;
		j++;
	}
}

static void	exec_move(t_stack *a, t_stack *b, int ca, int cb)
{
	while (ca > 0 && cb > 0 && ca-- && cb--)
		rr(a, b);
	while (ca < 0 && cb < 0 && ca++ && cb++)
		rrr(a, b);
	while (ca > 0 && ca--)
		ra(a);
	while (ca < 0 && ca++)
		rra(a);
	while (cb > 0 && cb--)
		rb(b);
	while (cb < 0 && cb++)
		rrb(b);
	pa(a, b);
}

void	sort_simple(t_stack *a, t_stack *b)
{
	int	mv[3];

	if (!a || !b || a->size <= 1 || b->size != 0 || is_sorted(a))
		return ;
	while (a->size > 3)
		pb(a, b);
	sort_three(a);
	while (b->size > 0)
	{
		pick_cheapest(a, b, mv);
		exec_move(a, b, mv[0], mv[1]);
	}
	mv[0] = find_min_pos(a);
	if (mv[0] > a->size / 2)
		mv[0] -= a->size;
	while (mv[0] > 0 && mv[0]--)
		ra(a);
	while (mv[0] < 0 && mv[0]++)
		rra(a);
}
