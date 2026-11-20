
#include "push_swap.h"

void	print_stack(char *name, t_stack *stack)
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

	printf("=== TESTE PB ===\n");
	pb(&a, &b);
	print_stacks(&a, &b);

	printf("=== TESTE PA ===\n");
	pa(&a, &b);
	print_stacks(&a, &b);

	printf("=== ESVAZIAR A COM PB ===\n");
	pb(&a, &b);
	pb(&a, &b);
	pb(&a, &b);
	print_stacks(&a, &b);

	printf("=== PB COM A VAZIA ===\n");
	pb(&a, &b);
	print_stacks(&a, &b);

	printf("=== ESVAZIAR B COM PA ===\n");
	while (b.head)
		pa(&a, &b);
	print_stacks(&a, &b);

	printf("=== PA COM B VAZIA ===\n");
	pa(&a, &b);
	print_stacks(&a, &b);

	return (0);
}
