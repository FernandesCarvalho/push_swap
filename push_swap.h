/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:37:33 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/27 17:20:56 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

typedef enum e_op
{
	OP_SA,
	OP_SB,
	OP_SS,
	OP_PA,
	OP_PB,
	OP_RA,
	OP_RB,
	OP_RR,
	OP_RRA,
	OP_RRB,
	OP_RRR
}	t_op;

typedef struct s_block
{
	int	read;
	int	write;
	int	rot_a;
	int	rot_b;
	int	size_a;
	int	size_b;
}	t_block;

typedef struct s_moves
{
	t_op	*ops;
	int		size;
	int		capacity;
	int		error;
}	t_moves;

typedef struct s_merge
{
	t_moves	a;
	t_moves	b;
	t_moves	result;
	int		read;
	int		write;
	int		start;
	int		size_a;
	int		size_b;
}	t_merge;

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*prev;
	struct s_node	*next;
}	t_node;

typedef struct s_stack
{
	t_node	*head;
	t_node	*tail;
	int		size;
	t_moves	*moves;
}	t_stack;

/* ALTERADO: novas estruturas para o novo quicksort do complex.            */
/* MOTIVO: o algoritmo antigo so sabia "grupo no topo de A" ou "grupo no   */
/* topo de B" e tinha de rebobinar rotacoes. O novo guarda tambem grupos   */
/* no fundo das pilhas, o que elimina esse rebobinar.                      */
/* t_loc   = onde esta um grupo (topo/fundo de A, topo/fundo de B).        */
/* t_part  = um grupo de numeros: posicao (loc) + quantidade (size).       */
/* t_split = as 3 partes que saem de dividir um grupo: min, mid e max.     */
typedef enum e_loc
{
	TOP_A,
	BOTTOM_A,
	TOP_B,
	BOTTOM_B
}	t_loc;

typedef struct s_part
{
	t_loc	loc;
	int		size;
}	t_part;

typedef struct s_split
{
	t_part	min;
	t_part	mid;
	t_part	max;
}	t_split;

typedef struct s_chunk
{
	int	start;
	int	end;
	int	mid;
}	t_chunk;

typedef enum e_strategy
{
	STRAT_ADAPTIVE,
	STRAT_SIMPLE,
	STRAT_MEDIUM,
	STRAT_COMPLEX
}	t_strategy;

typedef struct s_config
{
	t_strategy	strategy;
	int			bench;
	int			arg_start;
}	t_config;

typedef struct s_program
{
	t_stack		a;
	t_stack		b;
	t_config	config;
	t_strategy	used_strat;
	double		disorder;
	t_moves		moves;
	int			initial_size;
}	t_program;

typedef struct s_ops //
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	total;
}	t_ops;

//==============================================================
//                             ****     OPERATIONS     ****

//                    #SWAP

void		swap(t_stack *stack);
void		sa(t_stack *a);
void		sb(t_stack *b);
void		ss(t_stack *a, t_stack *b);

//                    #PUSH

void		push(t_stack *dst, t_stack *src);
void		pa(t_stack *a, t_stack *b);
void		pb(t_stack *a, t_stack *b);

//                    #ROTATE

void		rotate(t_stack *stack);
void		ra(t_stack *a);
void		rb(t_stack *b);
void		rr(t_stack *a, t_stack *b);

//                    #REVERSE ROTATE

void		reverse_rotate(t_stack *stack);
void		rra(t_stack *a);
void		rrb(t_stack *b);
void		rrr(t_stack *a, t_stack *b);

//======================================================================

//                             ****     PARSING     ****

int			ft_isdigit(int c);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		**ft_split(char const *s, char c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strdup(const char *s);

int			has_duplicate(t_stack *a, int value);
int			convert_numbers(char *str, int *result);
int			error_exit(t_stack *a, t_stack *b, char **split);
void		free_split(char **split);
int			parse_args(int argc, char **argv, t_stack *a, t_stack *b);

void		parse_flags(int argc, char **argv, t_config *config);
int			set_strategy(t_config *config, char *arg);
int			set_bench(t_config *config, char *arg);
void		init_config(t_config *config);

double		compute_disorder(t_stack *a);
t_strategy	choose_adaptive(double disorder);

t_ops		*get_ops_stats(void);
void		put_str_err(const char *s);
void		print_bench(t_config *cfg, t_strategy used_strat, double disorder);
void		put_nbr_err(long n);
void		record_op(int *op_count);
void		count_moves(t_moves *moves);

//==============================================================================

//                             ****     STACK     ****

void		add_node_back(t_stack *stack, t_node *new_node);
t_node		*new_node(int value);
void		free_stack(t_stack *stack);

//==============================================================================

//                             ****     SORTING     ****

//                  #INDEXING

void		assign_index(t_stack *a);

//                  #UTILS

int			is_sorted(t_stack *stack);//
int			find_min_pos(t_stack *a); //
int			ft_sqrt(int n);
void		push_min_to_b(t_stack *a, t_stack *b); //

//                  #SIMPLE

void		sort_simple(t_stack *a, t_stack *b);

//                  #MEDIUM
void		sort_medium(t_stack *a, t_stack *b);//
void		push_chunks_to_b(t_stack *a, t_stack *b);//

//                  #COMPLEX

/* ALTERADO: removidos get_pivot, partition_a e partition_b.               */
/* MOTIVO: faziam o quicksort de 2 metades, que gastava ~20% das ops a     */
/* rebobinar com rra/rrb. Substituidos pelas funcoes abaixo (quicksort de  */
/* 3 particoes): part_head, part_min, move_part e fix_part_loc ficam em    */
/* sort_complex_move.c (ficheiro novo); split_part em sort_complex_utils.c.*/
void		sort_complex(t_stack *a, t_stack *b);
t_node		*part_head(t_stack *a, t_stack *b, t_loc loc);
int			part_min(t_stack *a, t_stack *b, t_part *part);
void		move_part(t_stack *a, t_stack *b, t_loc from, t_loc to);
void		fix_part_loc(t_stack *a, t_stack *b, t_part *part);
void		split_part(t_stack *a, t_stack *b, t_part *part, t_split *split);
int			is_group_sorted(t_stack *stack, int size);
void		sort_three_group(t_stack *a);
void		sort_three_group_b(t_stack *a, t_stack *b);

//                  #SMALL

void		sort_two(t_stack *a);
void		sort_three(t_stack *a);
void		sort_four(t_stack *a, t_stack *b);
void		sort_five(t_stack *a, t_stack *b);

//==============================================================================

//                             ****     OPTIMIZE     ****

/* OPTIMIZE */

int			init_moves(t_moves *moves);
int			add_move(t_moves *moves, t_op op);
int			grow_moves(t_moves *moves);
void		print_moves(t_moves *moves);
void		free_moves(t_moves *moves);
void		optimize_pairs(t_moves *moves);
void		optimize_blocks(t_moves *moves, int size_a, int size_b);
void		optimize_moves(t_moves *moves, int initial_size);
void		optimize_merge(t_moves *moves, int initial_size);
void		normalize_block(t_block *block);
void		flush_rotations(t_moves *moves, t_block *block);
void		split_operation(t_op op, t_moves *a, t_moves *b);
void		merge_operations(t_moves *a, t_moves *b, t_moves *result);
void		process_block(t_merge *merge);
void		write_block(t_moves *moves, t_merge *merge);

#endif