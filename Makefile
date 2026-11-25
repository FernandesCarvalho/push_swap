# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fercarva <fercarva@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/21 17:33:56 by jeanbarr          #+#    #+#              #
#    Updated: 2026/11/25 12:43:42 by fercarva         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= push_swap
CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror
RM 		= rm -f

INCLUDES = -I.

SRCS	= main_jeanlezy.c \
			stack/new_node_and_back.c \
			stack/free_stack.c \
			parsing/convert_numbers.c \
			parsing/error_exit.c \
			parsing/ft_split.c \
			parsing/mini_libft.c \
			parsing/has_duplicate.c \
			parsing/parse_args.c \
			parsing/parse_flags.c \
			operations/push.c \
			operations/rotate.c \
			operations/reverse_rotate.c \
			operations/swap.c \
			sorting/chunk_utils.c \
			sorting/indexing.c \
			sorting/sort_complex_utils.c \
			sorting/sort_complex_group.c \
			sorting/sort_complex.c \
			sorting/sort_medium.c \
			sorting/sort_simple.c \
			sorting/sort_small.c \
			sorting/sort_utils.c \
	
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re