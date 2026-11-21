
#include "push_swap.h"
#include <stdio.h>

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

	printf("=== TESTE RA ===\n");
	ra(&a);
	print_stacks(&a, &b);

	printf("=== TESTE RB ===\n");
	rb(&b);
	print_stacks(&a, &b);

	printf("=== TESTE RR ===\n");
	rr(&a, &b);
	print_stacks(&a, &b);

	printf("=== RR NOVAMENTE ===\n");
	rr(&a, &b);
	print_stacks(&a, &b);

	printf("=== TESTE COM A VAZIA ===\n");
	while (a.head)
		pb(&a, &b);
	ra(&a);
	rr(&a, &b);
	print_stacks(&a, &b);

	printf("=== TESTE COM UM NO ===\n");
	while (b.size > 1)
		pa(&a, &b);
	rb(&b);
	print_stacks(&a, &b);

	return (0);
}
