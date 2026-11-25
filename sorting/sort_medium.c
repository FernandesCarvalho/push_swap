/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 00:50:44 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/25 08:30:49 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"
// Chunk Sort: divide os números em grupos (chunks) 
// e movimenta esses grupos estrategicamente entre A e B.
void	sort_medium(t_stack *a, t_stack *b)
{
	//int	chunk_size;
	
	if (!a || !b || is_sorted(a))
	{
		
	}
}
//O algoritmo vai:
/* 1. Procurar na stack A um elemento cujo índice pertença ao chunk atual.
2. Se encontrar um elemento adequado, executa pb().
3. Se o elemento for relativamente pequeno dentro do chunk, executa rb() para o 
	colocar mais abaixo na stack B.
4. Se não encontrar um elemento adequado no topo, executa ra().
5. Repete até todos os elementos estarem em B.
6. Depois começa a devolver os elementos de B para A, procurando sempre o maior índice. */

/* O value guarda o número original.
O index guarda a posição desse número quando todos os valores são ordenados. */