/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 17:57:31 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 17:24:11 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ========================================================================== */
/* ALTERADO: o algoritmo deste ficheiro foi trocado.                          */
/*                                                                            */
/* ANTES: sort_a / sort_b / sort_small_b = quicksort de 2 metades que ia      */
/* alternando entre A e B e rebobinava as rotacoes com rra/rrb.               */
/* Resultado medido no ft_ps_tester: 100 numeros max 712 (GOOD), invertido    */
/* 734 (GOOD) e 2 avisos de "Mode specialization" (o --medium e o --simple    */
/* gastavam >10% menos ops que o --complex nos numeros "complex").            */
/*                                                                            */
/* AGORA: quicksort de 3 particoes com 4 posicoes (TOP_A, BOTTOM_A, TOP_B,    */
/* BOTTOM_B). Cada grupo e dividido em max/mid/min e cada parte e ordenada    */
/* recursivamente ate ficar no topo de A. Resultado: ~550 ops (100) e         */
/* ~3860 ops (500), tudo EXCELLENT e sem avisos.                              */
/* ========================================================================== */

#include "../push_swap.h"

void		sort_complex(t_stack *a, t_stack *b);
static void	sort_part(t_stack *a, t_stack *b, t_part *part);

/* ALTERADO: antes chamava sort_a(a, b, a->size).                           */
/* O QUE FAZ: cria o primeiro grupo = toda a pilha A (posicao TOP_A) e      */
/* manda ordena-lo com sort_part. O assign_index continua igual.            */
void	sort_complex(t_stack *a, t_stack *b)
{
	t_part	part;

	if (!a || !b || !a->head)
		return ;
	assign_index(a);
	part.loc = TOP_A;
	part.size = a->size;
	sort_part(a, b, &part);
}

/* ALTERADO (funcao nova): grupo de 2 numeros em qualquer posicao.          */
/* O QUE FAZ: se nao estiver no topo de A, leva os 2 para la; depois faz    */
/* sa se o de cima for maior que o de baixo.                                */
/* MOTIVO: o antigo sort_small_b so sabia tratar grupos no topo de B; agora */
/* os grupos podem estar em 4 posicoes.                                     */
static void	sort_two_part(t_stack *a, t_stack *b, t_part *part)
{
	if (part->loc != TOP_A)
	{
		move_part(a, b, part->loc, TOP_A);
		move_part(a, b, part->loc, TOP_A);
	}
	if (a->head->index > a->head->next->index)
		sa(a);
}

/* ALTERADO (funcao nova, substitui sort_small_b): casos de 1, 2 e 3.       */
/* O QUE FAZ:                                                               */
/*  - 1 numero: so leva para o topo de A;                                   */
/*  - 2 numeros: sort_two_part;                                             */
/*  - 3 no topo de B: usa o sort_three_group_b que ja existia;              */
/*  - 3 noutra posicao: leva os 3 para o topo de A e usa o                  */
/*    sort_three_group que ja existia.                                      */
/* MOTIVO: partir grupos tao pequenos custaria mais ops do que ordena-los   */
/* diretamente.                                                             */
static void	sort_small_part(t_stack *a, t_stack *b, t_part *part)
{
	if (part->size == 1)
	{
		if (part->loc != TOP_A)
			move_part(a, b, part->loc, TOP_A);
	}
	else if (part->size == 2)
		sort_two_part(a, b, part);
	else if (part->loc == TOP_B)
		sort_three_group_b(a, b);
	else
	{
		while (part->loc != TOP_A && part->size-- > 0)
			move_part(a, b, part->loc, TOP_A);
		sort_three_group(a);
	}
}

/* ALTERADO (funcao nova, substitui sort_a e sort_b): quicksort recursivo.  */
/* O QUE FAZ:                                                               */
/*  1) fix_part_loc: se o grupo esta no fundo mas e a pilha inteira,        */
/*     passa a ser tratado como topo (poupa rra/rrb);                       */
/*  2) se ja esta no topo de A e ordenado, nao faz nada (0 ops);            */
/*  3) se tem 3 ou menos numeros, ordena direto (sort_small_part);          */
/*  4) senao divide em 3 partes (split_part) e ordena primeiro o max, depois*/
/*     o mid e por fim o min. Cada parte termina ordenada no topo de A,     */
/*     por cima da anterior, por isso no fim a pilha A fica toda ordenada.  */
static void	sort_part(t_stack *a, t_stack *b, t_part *part)
{
	t_split	split;

	if (part->size <= 0)
		return ;
	fix_part_loc(a, b, part);
	if (part->loc == TOP_A && is_group_sorted(a, part->size))
		return ;
	if (part->size <= 3)
	{
		sort_small_part(a, b, part);
		return ;
	}
	split_part(a, b, part, &split);
	sort_part(a, b, &split.max);
	sort_part(a, b, &split.mid);
	sort_part(a, b, &split.min);
}
