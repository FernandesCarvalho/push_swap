/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 11:57:20 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/24 13:07:44 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"
int	find_max_pos(t_stack *b)
{
	int		pos;// pos; guarda a posição (começando em 0 no topo) 
			// onde foi encontrado o maior index até ao momento.
	int		val; //guarda o maior index encontrado até ao momento
	int		i;
	t_node	*current;

	if (!b || !b->head)
		return (0);
	current = b->head;
	i = 0;
	val = current->index; //assume que index do primeiro node = maior
	pos = 0; // como inicial ta no topo posicao inicial 0
	while (current)
	{
		if (current->index > val) // compara o index do nó atual com o maior index 
								  // registrado em val
		{
			val = current->index; // atualiza val com esse novo maior index
			pos = i; // guarda em pos a posição i em que esse novo máximo foi encontrado
		}
		current = current->next;
		i++;
	}
	return (pos);
}
void	push_max_to_a(t_stack *a, t_stack *b)
{
	int	pos;
	int	i;

	i = 0;
	if (!a || !b || b->size == 0)
		return ;
	pos = find_max_pos(b);
	if (pos <= b->size / 2)
	{
		while(i < pos)
		{
			rb(b);
			i++;	
		}
	}
	else
	{
		i = b->size;
		while(i > pos)
		{
			rrb(b);
			i--;	
		}
	}
	pa(a, b);
	/* Primeiro, distribuímos os elementos de A para B usando chunks.
	Depois, A fica vazia e B contém todos os elementos.
	Agora queremos devolver os elementos de B para A, começando pelo maior. */
}

push_chunks_to_b(): envia os números de A para B.
/* Enquanto A não estiver vazia:
    Se o index do primeiro elemento pertence ao chunk:
        pb()
    Se o index for pequeno:
        rb()
    Caso contrário:
        ra() */

push_back_to_a(): devolve os números de B para A na ordem correta.
