/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 19:02:50 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/24 18:28:48 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"
/* Arquivo que pega args da linha de comando, divide espacos, valida/converte
   e monta a pilha 'a' no final*/

//Coloca o número no fim da pilha
static int	append_to_a(t_stack *a, int value)
{
	t_node	*node;

	node = new_node(value);
	if (!node)
		return (0);
	if (!a->head)
	{
		a->head = node;
		a->tail = node;
	}
	else
	{
		node->prev = a->tail;
		a->tail->next = node;
		a->tail = node;
	}
	a->size++;
	return (1);
}

//Valida, converte e insere um único número
static int	process_token(t_stack *a, t_stack *b, char *token, char **split)
{
	int	val;
	
	if (!convert_numbers(token, &val))
		return (error_exit(a, b, split));
	if (has_duplicate(a, val))
		return (error_exit(a, b, split));
	// tenta adicionar 'val' no fim de a
	if (!append_to_a(a, val))
		return (error_exit(a, b, split));
	return (0);
}
// Percorre argv/argc e coordena o split
int	parse_args(int argc, char **argv, t_stack *a, t_stack *b, int start)
{
	char	**split;
	int		i;
	int		j;

	if (!a || !b)
		return (1);
	i = start;
	while (i < argc)
	{
		split = ft_split(argv[i], ' ');
		if (!split || !split[0])
			return (error_exit(a, b, split));
		j = 0;
		while (split[j])
		{
			if (process_token(a, b, split[j], split))
				return (1);
			j++;
		}
		free_split(split);
		i++;
	}
	return (0);
}
