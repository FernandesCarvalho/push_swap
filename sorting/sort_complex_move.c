/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_move.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 18:30:00 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/28 00:30:24 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ========================================================================== */
/* ALTERADO: ficheiro novo (adicionado tambem no Makefile).                   */
/*                                                                            */
/* MOTIVO: o novo quicksort do complex trabalha com grupos (t_part) que podem */
/* estar em 4 posicoes: topo ou fundo de A e topo ou fundo de B. Estas        */
/* funcoes sao o "motor" que le e move numeros de/para essas 4 posicoes.      */
/* Ficaram num ficheiro separado por causa do limite de 5 funcoes por         */
/* ficheiro da Norminette.                                                    */
/* ========================================================================== */

#include "../push_swap.h"

/* ALTERADO (funcao nova): devolve o proximo numero a sair do grupo.       */
/* O QUE FAZ: grupo no topo -> head da pilha; grupo no fundo -> tail.      */
/* MOTIVO: o split precisa de ver o index do numero antes de o mover.      */
t_node	*part_head(t_stack *a, t_stack *b, t_loc loc)
{
	if (loc == TOP_A)
		return (a->head);
	if (loc == BOTTOM_A)
		return (a->tail);
	if (loc == TOP_B)
		return (b->head);
	return (b->tail);
}

/* ALTERADO (funcao nova, substitui a parte do get_pivot que achava o     */
/* minimo): devolve o menor index do grupo.                                */
/* O QUE FAZ: percorre os "size" numeros do grupo (para baixo se esta no   */
/* topo, para cima se esta no fundo).                                      */
/* MOTIVO: os indexes de um grupo sao sempre seguidos (min .. min+size-1), */
/* por isso min + percentagem do tamanho da os pivots sem ordenar nada.    */
int	part_min(t_stack *a, t_stack *b, t_part *part)
{
	t_node	*node;
	int		min;
	int		i;

	node = part_head(a, b, part->loc);
	min = node->index;
	i = 0;
	while (node && i < part->size)
	{
		if (node->index < min)
			min = node->index;
		if (part->loc == TOP_A || part->loc == TOP_B)
			node = node->next;
		else
			node = node->prev;
		i++;
	}
	return (min);
}

/* ALTERADO (funcao nova): move 1 numero da posicao "from" para "to".     */
/* O QUE FAZ, por passos:                                                  */
/*  1) se o numero esta no fundo, traz para o topo (rra ou rrb);           */
/*  2) se o destino e a outra pilha, empurra (pa ou pb);                   */
/*  3) se o destino e o fundo, roda (ra ou rb).                            */
/* Ex.: BOTTOM_B -> BOTTOM_A = rrb, pa, ra.                                */
/* MOTIVO: uma so funcao cobre as 12 combinacoes origem/destino.           */
void	move_part(t_stack *a, t_stack *b, t_loc from, t_loc to)
{
	if (from == BOTTOM_A)
		rra(a);
	else if (from == BOTTOM_B)
		rrb(b);
	if ((from == TOP_A || from == BOTTOM_A) && (to == TOP_B || to == BOTTOM_B))
		pb(a, b);
	else if ((from == TOP_B || from == BOTTOM_B)
		&& (to == TOP_A || to == BOTTOM_A))
		pa(a, b);
	if (to == BOTTOM_A)
		ra(a);
	else if (to == BOTTOM_B)
		rb(b);
}

/* ALTERADO (funcao nova): corrige a posicao do grupo.                    */
/* O QUE FAZ: se o grupo esta no fundo mas ocupa a pilha inteira, entao    */
/* o fundo e o topo sao o mesmo grupo -> passa a ser tratado como topo.    */
/* MOTIVO: ler pelo topo nao custa nada; pelo fundo custava 1 rra/rrb      */
/* extra por numero.                                                       */
void	fix_part_loc(t_stack *a, t_stack *b, t_part *part)
{
	if (part->loc == BOTTOM_A && a->size == part->size)
		part->loc = TOP_A;
	else if (part->loc == BOTTOM_B && b->size == part->size)
		part->loc = TOP_B;
}
