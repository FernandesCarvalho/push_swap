/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 19:02:50 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/22 20:33:51 by jeanbarr         ###   ########.fr       */
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
	// se a pilha a ainda está vazia, 
	// o novo nó vira tanto o primeiro (head) quanto o último (tail)
	if (!a->head)
	{
		a->head = node;
		a->tail = node;
	}
	//se já existem nós, encadeia o node depois do tail atual: 
	// aponta node->prev para o antigo tail, liga a->tail->next 
	// no node e atualiza a->tail = node
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
	
	// confere se e numero int valido, converte str p/ int guardando em &val
	// checa under e overflow
	if (!convert_numbers(token, &val))
		return (error_exit(a, b, split));
	// verifica duplicados
	if (has_duplicate(a, val))
		return (error_exit(a, b, split));
	// tenta adicionar 'val' no fim de a
	if (!append_to_a(a, val))
		return (error_exit(a, b, split));
	return (0);
}
// Percorre argv/argc e coordena o split
int	parse_args(int argc, char **argv, t_stack *a, t_stack *b)
{
	char	**split;
	int		i;
	int		j;

	if (!a || !b)
		return (1);
	i = 1;
	while (i < argc)
	{
		// separa o argumento atual por espaços
		split = ft_split(argv[i], ' ');
		if (!split || !split[0])
			return (error_exit(a, b, split));
		j = 0;
		while (split[j])
		{
			// envia 'split[j]' para 'process_token', que vai converter o texto em 'int' 
			// (com checagem de formato e overflow/underflow), verificar se já existe 
			// duplicado em a e adicionar o nó ao final de 'a'
			if (process_token(a, b, split[j], split))
				return (1);
			j++;
		}
		free_split(split);
		i++;
	}
	return (0);
}
