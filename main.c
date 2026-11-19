/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:57:47 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/19 13:11:56 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int main(void)
{
	t_stack a;
    t_node node1;
	t_node node2;
	t_node node3;
	t_node *current;

	a.head = NULL;
	a.tail = NULL;
	a.size = 0;
	

	node1.value = 42;
	node2.value = 21;
	node3.value = 10;

	//Sem nós
	printf("Teste sem nós:\n");
	swap();

	
	add_node_back(&a, &node1);
	
	current = a.head;
	add_node_back(&a, &node2);
	add_node_back(&a, &node3);
	
	current = a.head;
	printf("Teste com 3 nós:\n");
	while (current)
	{
		printf("Valor do nó: %i\n", current->value);
		current = current->next;
	}

    return (0);
}