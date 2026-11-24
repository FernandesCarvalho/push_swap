# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/21 17:33:56 by jeanbarr          #+#    #+#              #
#    Updated: 2026/11/24 18:12:58 by jeanbarr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= push_swap
CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror
RM 		= rm -f

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

INCLUDES = -I. -I$(LIBFT_DIR)

SRCS	= main_jeanlezy.c \
			stack/add_node_back.c \
			stack/free_stack.c \
			stack/new_node.c \
			parsing/convert_numbers.c \
			parsing/error_exit.c \
			parsing/has_duplicate.c \
			parsing/parse_args.c \
			parsing/parse_flags.c \
			operations/push.c \
			operations/rotate.c \
			operations/reverse_rotate.c \
			operations/swap.c \
			sorting/sort_utils.c \
			sorting/sort_small.c \
			sorting/sort_simple.c \
			sorting/sort_medium.c \
			sorting/indexing.c \
			sorting/chunk_utils.c
	
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re