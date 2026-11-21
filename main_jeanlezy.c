#include "push_swap.h"

static void	init_stack(t_stack *stack)
{
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
}

static int	process_token(char *token, t_stack *a, t_stack *b, char **split)
{
	int		val;
	t_node	*node;

	if (!validate_number(token) || !convert_numbers(token, &val))
		return (error_exit(a, b, split));
	if (has_duplicate(a, val))
		return (error_exit(a, b, split));
	node = new_node(val);
	if (!node)
		return (error_exit(a, b, split));
	add_node_back(a, node);
	return (0);
}

static int	parse_args(int argc, char **argv, t_stack *a, t_stack *b)
{
	char	**split;
	int		i;
	int		j;

	i = 1;
	while (i < argc)
	{
		if (!argv[i][0])
			return (error_exit(a, b, NULL));
		split = ft_split(argv[i], ' ');
		if (!split || !split[0])
			return (error_exit(a, b, split));
		j = 0;
		while (split[j])
		{
			if (process_token(split[j], a, b, split))
				return (1);
			j++;
		}
		free_split(split);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;

	if (argc == 1)
		return (0);
	init_stack(&a);
	init_stack(&b);
	if (parse_args(argc, argv, &a, &b))
		return (1);
	free_stack(&a);
	free_stack(&b);
	return (0);
}