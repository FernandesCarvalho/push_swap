/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 15:09:50 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 17:15:07 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ========================================================================== */
/* ALTERADO: todo o conteudo deste ficheiro foi substituido.                  */
/*                                                                            */
/* ANTES: get_pivot, rotation_to_a, partition_a, rotation_to_b, partition_b.  */
/* Dividiam cada grupo em 2 metades (pivot = mediana). A metade que ficava    */
/* tinha de ser rodada (ra/rb) e depois "rebobinada" (rra/rrb) para voltar    */
/* ao topo. Esse rebobinar gastava cerca de 20% das operacoes (~1000 ops em   */
/* 500 numeros) e deixava o complex no limite do EXCELLENT (max 712 em 100).  */
/*                                                                            */
/* AGORA: particao em 3 partes (min / mid / max) com 2 pivots. Cada parte e   */
/* enviada para uma posicao diferente (topo ou fundo de A ou de B), por isso  */
/* nenhum numero precisa de voltar para tras: cada numero e movido 1 vez por  */
/* particao. Continua O(n log n) (log base 3 em vez de base 2).               */
/* ========================================================================== */

#include "../push_swap.h"

/* ALTERADO (funcao nova): define para onde vai cada uma das 3 partes,      */
/* conforme a posicao de origem do grupo.                                   */
/* MOTIVO: as partes nunca podem voltar para a posicao de origem (iam       */
/* misturar-se com os numeros ainda por ler), e os maiores (max) vao sempre */
/* para A, porque e la que tem de acabar o resultado ordenado.              */
/*   origem TOP_A    -> max: BOTTOM_A | mid: TOP_B    | min: BOTTOM_B       */
/*   origem BOTTOM_A -> max: TOP_A    | mid: TOP_B    | min: BOTTOM_B       */
/*   origem TOP_B    -> max: TOP_A    | mid: BOTTOM_A | min: BOTTOM_B       */
/*   origem BOTTOM_B -> max: TOP_A    | mid: BOTTOM_A | min: TOP_B          */
static void	set_split_loc(t_loc loc, t_split *split)
{
	split->min.size = 0;
	split->mid.size = 0;
	split->max.size = 0;
	split->min.loc = BOTTOM_B;
	split->mid.loc = TOP_B;
	split->max.loc = BOTTOM_A;
	if (loc == BOTTOM_A)
		split->max.loc = TOP_A;
	else if (loc == TOP_B)
	{
		split->mid.loc = BOTTOM_A;
		split->max.loc = TOP_A;
	}
	else if (loc == BOTTOM_B)
	{
		split->min.loc = TOP_B;
		split->mid.loc = BOTTOM_A;
		split->max.loc = TOP_A;
	}
}

/* ALTERADO (funcao nova): percentagem do grupo onde fica cada pivot.       */
/* MOTIVO: o custo de mover um numero depende da origem e do destino        */
/* (ex.: de TOP_A para BOTTOM_B custa pb + rb = 2 ops; para TOP_B so 1).    */
/* Em vez de cortar sempre em 1/3 e 2/3, cada origem tem os seus cortes,    */
/* para mandar mais numeros para os destinos baratos. Os valores foram      */
/* afinados por testes (100 e 500 numeros aleatorios):                      */
/* 1/3-2/3 fixo dava ~605 ops (100) e ~4105 (500); assim da ~550 e ~3860.   */
static int	pivot_percent(t_loc loc, int is_big)
{
	if (loc == TOP_A && is_big)
		return (58);
	if (loc == TOP_A)
		return (18);
	if (loc == BOTTOM_A && is_big)
		return (66);
	if (loc == BOTTOM_A)
		return (22);
	if (loc == TOP_B && is_big)
		return (62);
	if (loc == TOP_B)
		return (42);
	if (is_big)
		return (62);
	return (46);
}

/* ALTERADO (funcao nova): move 1 numero para a parte de destino e conta    */
/* quantos numeros essa parte ja tem (o tamanho e preciso na recursao).     */
static void	send_to(t_stack *a, t_stack *b, t_loc from, t_part *dest)
{
	move_part(a, b, from, dest->loc);
	dest->size++;
}

/* ALTERADO (funcao nova, substitui partition_a e partition_b).             */
/* O QUE FAZ: le os "size" numeros do grupo a partir da sua posicao e       */
/* separa-os pelos 2 pivots:                                                */
/*   index >= pivot_big            -> parte max                             */
/*   pivot_small <= index < big    -> parte mid                             */
/*   index < pivot_small           -> parte min                             */
/* No fim, split tem as 3 partes com a posicao e o tamanho de cada uma.     */
void	split_part(t_stack *a, t_stack *b, t_part *part, t_split *split)
{
	int	pivot_small;
	int	pivot_big;
	int	index;
	int	i;

	set_split_loc(part->loc, split);
	pivot_small = part_min(a, b, part)
		+ (part->size * pivot_percent(part->loc, 0)) / 100;
	pivot_big = part_min(a, b, part)
		+ (part->size * pivot_percent(part->loc, 1)) / 100;
	i = 0;
	while (i < part->size)
	{
		index = part_head(a, b, part->loc)->index;
		if (index >= pivot_big)
			send_to(a, b, part->loc, &split->max);
		else if (index >= pivot_small)
			send_to(a, b, part->loc, &split->mid);
		else
			send_to(a, b, part->loc, &split->min);
		i++;
	}
}
