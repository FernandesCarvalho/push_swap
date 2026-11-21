
#include "push_swap.h"

int	main(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		printf("argv[%i] = %s\n", i, av[i]);
		i++;
	}
	return (0);
}

/* void	print_stack(char *name, t_stack *stack)
{
	t_node	*current;

	printf("%s (size: %d): ", name, stack->size);
	current = stack->head;
	while (current)
	{
		printf("%d ", current->value);
		current = current->next;
	}
	printf("\n");
}

void	print_stacks(t_stack *a, t_stack *b)
{
	print_stack("A", a);
	print_stack("B", b);
	printf("\n");
}

int	main(void)
{
	t_stack	a = {0};
	t_stack	b = {0};
	t_node	a1 = {.value = 42};
	t_node	a2 = {.value = 21};
	t_node	a3 = {.value = 10};
	t_node	b1 = {.value = 8};
	t_node	b2 = {.value = 5};

	add_node_back(&a, &a1);
	add_node_back(&a, &a2);
	add_node_back(&a, &a3);
	add_node_back(&b, &b1);
	add_node_back(&b, &b2);

	printf("=== ESTADO INICIAL ===\n");
	print_stacks(&a, &b);

	printf("=== TESTE RRA ===\n");
	rra(&a);
	print_stacks(&a, &b);

	printf("=== TESTE RRB ===\n");
	rrb(&b);
	print_stacks(&a, &b);

	printf("=== TESTE RRR ===\n");
	rrr(&a, &b);
	print_stacks(&a, &b);

	printf("=== RRR NOVAMENTE ===\n");
	rrr(&a, &b);
	print_stacks(&a, &b);

	printf("=== TESTE COM A VAZIA ===\n");
	while (a.head)
		pb(&a, &b);
	rra(&a);
	rrr(&a, &b);
	print_stacks(&a, &b);

	printf("=== TESTE COM UM NO ===\n");
	while (b.size > 1)
		pa(&a, &b);
	rrb(&b);
	print_stacks(&a, &b);

	return (0);
} */
