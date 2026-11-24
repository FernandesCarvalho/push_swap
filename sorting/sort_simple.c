/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 23:26:23 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/24 00:10:00 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

//organiza os numeros de forma simples, porém 
//nao funciona com muitos numeros 
void	sort_simple(t_stack *a, t_stack *b)
{
	if (!a || !b || a->size <= 1 || b->size != 0)
		return ;
	if (is_sorted(a))
		return ;
	while (a->size > 2)
		push_min_to_b(a, b);
	sort_two(a);
	while (b->size > 0)
		pa(a, b);
}