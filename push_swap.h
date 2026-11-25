/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:37:33 by fercarva          #+#    #+#             */
/*   Updated: 2026/11/25 12:47:52 by fercarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>

typedef struct s_node
{
	int	value;
	int	index;
	struct s_node	*prev;
	struct s_node	*next;
} t_node;

typedef struct s_stack
{
	t_node	*head;
	t_node	*tail;
	int		size;
} t_stack;

typedef struct s_chunk
{
	int start;
	int end;
	int mid;
}	t_chunk;

typedef enum e_strategy // Guarda a estratégia escolhida
{
	STRAT_ADAPTIVE,
	STRAT_SIMPLE,
	STRAT_MEDIUM,
	STRAT_COMPLEX
}	t_strategy;

typedef struct s_config
{
	t_strategy	strategy;
	int			bench; //Indica se o benchmark está ativo
	int			arg_start; //Indica onde começam os números em argv
}	t_config;
//========================================================================================================

//                             ****     OPERATIONS     ****

//                    #SWAP

void	swap(t_stack *stack);
void	sa(t_stack *a);
void	sb(t_stack *b);
void	ss(t_stack *a, t_stack *b);

//                    #PUSH

void	push(t_stack *dst, t_stack *src);
void	pa(t_stack *a, t_stack *b);
void	pb(t_stack *a, t_stack *b);

//                    #ROTATE

void	rotate(t_stack *stack);
void	ra(t_stack *a);
void	rb(t_stack *b);
void	rr(t_stack *a, t_stack *b);

//                    #REVERSE ROTATE

void	reverse_rotate(t_stack *stack);
void	rra(t_stack *a);
void	rrb(t_stack *b);
void	rrr(t_stack *a, t_stack *b);

//========================================================================================================

//                             ****     PARSING     ****

int		ft_isdigit(int c);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);

int		has_duplicate(t_stack *a, int value);
int		convert_numbers(char *str, int *result);
int		error_exit(t_stack *a, t_stack *b, char **split);
void	free_split(char **split);
int		parse_args(int argc, char **argv, t_stack *a, t_stack *b, int start);

void	parse_flags(int argc, char **argv, t_config *config); //
int		set_strategy(t_config *config, char *arg);//
int		set_bench(t_config *config, char *arg); //
void	init_config(t_config *config); //


//========================================================================================================

//                             ****     STACK     ****

void	add_node_back(t_stack *stack, t_node *new_node);
t_node	*new_node(int value);
void	free_stack(t_stack *stack);

//========================================================================================================

//                             ****     SORTING     ****

//                  #INDEXING
void	assign_index(t_stack *a);

//                  #UTILS
int		is_sorted(t_stack *stack);//
int		find_min_pos(t_stack *a); //
int		ft_sqrt(int n);
void	push_min_to_b(t_stack *a, t_stack *b); //
int		find_max_pos(t_stack *b); //

//                  #SIMPLE
void	sort_simple(t_stack *a, t_stack *b);

//                  #MEDIUM
void	sort_medium(t_stack *a, t_stack *b);//
void	push_max_to_a(t_stack *a, t_stack *b);//
void	push_chunk_node(t_stack *a, t_stack *b, int *pushed, t_chunk *chunk);//
void	push_chunks_to_b(t_stack *a, t_stack *b);//
void	push_back_to_a(t_stack *a, t_stack *b);//

//                  #COMPLEX
int		get_pivot(t_stack *stack, int size);
int		partition_a(t_stack *a, t_stack *b, int size, int pivot);
int		is_group_sorted(t_stack *stack, int size);
int		partition_b(t_stack *a, t_stack *b, int size, int pivot);
void	sort_complex(t_stack *a, t_stack *b);
void	sort_three_group(t_stack *a);
void	sort_three_group_b(t_stack *a, t_stack *b);


//                  #SMALL
void	sort_two(t_stack *a);
void	sort_three(t_stack *a);
void	sort_four(t_stack *a, t_stack *b);
void	sort_five(t_stack *a, t_stack *b);

#endif