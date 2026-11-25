/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 00:13:48 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/25 12:38:44 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

//FUNÇÃO QUE VERIFICA NÓ POR NÓ, ASSINALANDO ONDE SEVERIA SER A SUA
//POSIÇÃO, VERIFICANDO DA MENOR PARA A MAIOR PASSANDO DEU INDEX PARA 
// O T_NODE->INDEX;
// técnica chamada coordinate compression
// A função compara cada número com todos os outros e atribui a sua 
// posição na ordem crescente:
void	assign_index(t_stack *a)
{
	int		index;
	t_node	*check;
	t_node	*current;

	if (!a || a->size < 1)
		return ;
	current = a->head;
	while (current)
	{
		check = a->head;
		index = 0;
		while (check)
		{
			if (current->value > check->value)
				index++;
			check = check->next;
		}
		current->index = index;
		current = current->next;
	}
}
