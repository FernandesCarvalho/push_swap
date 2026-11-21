# Push_swap — Estado atualizado do projeto

> Documento de acompanhamento atualizado em **21/11/2026** com as implementações, correções e novos ficheiros adicionados após a estruturação inicial. Serve para sincronizar o estado atual da branch entre os membros da dupla.

---

## 1. Resumo do que foi feito nesta etapa

1. **Infraestrutura e Compilação (`Makefile` e `push_swap.h`):**
   * Criado e configurado o `Makefile` na raiz compilando com `cc -Wall -Wextra -Werror`, integrando a compilação da `libft` (`$(MAKE) -C libft`) e evitando *relink* desnecessário.
   * Atualizado `push_swap.h` com todos os protótipos de *parsing*, limpeza de memória e helpers de *stack*.

2. **Correção e finalização do `convert_numbers()`:**
   * Corrigido o loop de leitura (`while (*str)`) e a verificação de *overflow* para `INT_MAX` e `INT_MIN` (`check_overflow`), evitando *overflow* de `long` em strings grandes e aceitando `-2147483648` e `2147483647`.
   * Ajustado o tamanho e formatação da função para cumprir o limite de 25 linhas da Norminette.

3. **Novos ficheiros de Parsing e Tratamento de Erros (`parsing/`):**
   * `parsing/has_duplicate.c`: implementada `has_duplicate(t_stack *a, int value)` para rejeitar números repetidos.
   * `parsing/error.c`: implementadas `free_split(char **split)` e `error_exit(t_stack *a, t_stack *b, char **split)`, que liberta toda a memória alocada e escreve `Error\n` no *standard error* (`fd 2`).
   * `parsing/validate_number.c`: ajustada a formatação (Norminette) para validação de sinais e dígitos.

4. **Novos Helpers de Gestão de Memória e Nós (`stack/`):**
   * `stack/new_node.c`: implementada `new_node(int value)` para alocação dinâmica segura de `t_node`.
   * `stack/free_stack.c`: implementada `free_stack(t_stack *stack)` para libertação completa dos nós e reinicialização de `head`, `tail` e `size`.

5. **Proteção nas Operações (`operations/`):**
   * Adicionadas verificações de ponteiros nulos (`!stack`, `!dst`, `!src`, `!a`, `!b`) em `push.c`, `rotate.c`, `reverse_rotate.c` e `swap.c`.
   * Em `swap.c` (`ss`), garantido que cada pilha só troca se tiver `size >= 2` e imprime `ss\n` apenas uma vez se houver movimento.

6. **Integração do Parsing no `main` (`main.c` / `main_jeanlezy.c`):**
   * Implementado o fluxo completo de leitura de `argc`/`argv` (`init_stack`, `parse_args`, `process_token`), suportando argumentos separados (`1 2 3`), strings com múltiplos números (`"1 2 3"`) ou combinação de ambos.
   * Tratamento de entrada vazia (`argc == 1` encerra sem saída), strings vazias (`""`), strings só com espaços (`"   "`), valores inválidos/fora de `int` e duplicados com limpeza total sem *memory leaks*.

---

## 2. Estrutura atual de pastas e ficheiros

```text
42-push_swap/
├── Makefile
├── README.md
├── push_swap.h
├── main.c                      # (e main_jeanlezy.c usado na transição/testes)
├── libft/                      # biblioteca de apoio
├── parsing/
│   ├── validate_number.c       # validate_number()
│   ├── convert_numbers.c       # check_overflow(), convert_numbers()
│   ├── has_duplicate.c         # has_duplicate()
│   └── error.c                 # free_split(), error_exit()
├── stack/
│   ├── add_node_back.c         # add_node_back()
│   ├── new_node.c              # new_node()
│   └── free_stack.c            # free_stack()
└── operations/
    ├── swap.c                  # swap(), sa(), sb(), ss()
    ├── push.c                  # push(), pa(), pb()
    ├── rotate.c                # rotate(), ra(), rb(), rr()
    └── reverse_rotate.c        # reverse_rotate(), rra(), rrb(), rrr()
```

---

## 3. Tabela de funções implementadas e estado atual

| Pasta / Ficheiro | Funções | Estado | Observações |
|---|---|---|---|
| `Makefile` | Regras `all`, `$(NAME)`, `clean`, `fclean`, `re` | Pronto | Confirmar inclusão de `stack/new_node.c` em `SRCS` e remover `main_jeanlezy.c` após sincronizar `main.c`. |
| `main.c` | `init_stack`, `process_token`, `parse_args`, `main` | Implementado | Faz parsing completo de `argc`/`argv`, valida, cria nós e limpa memória. |
| `parsing/validate_number.c` | `validate_number(char *str)` | Implementado | Valida sinal inicial e dígitos. |
| `parsing/convert_numbers.c` | `check_overflow`, `convert_numbers` | Corrigido | Converte para `int` com proteção de `INT_MIN` / `INT_MAX` e <= 25 linhas. |
| `parsing/has_duplicate.c` | `has_duplicate(t_stack *a, int value)` | Implementado | Verifica valores repetidos antes de inserir na pilha `a`. |
| `parsing/error.c` | `free_split`, `error_exit` | Implementado | Limpa `split`, `a` e `b`, escreve `Error\n` no `fd 2` e devolve `1`. |
| `stack/new_node.c` | `new_node(int value)` | Implementado | Aloca e inicializa um nó `t_node`. |
| `stack/add_node_back.c` | `add_node_back(t_stack *stack, t_node *new_node)` | Implementado | Adiciona nó ao fim da lista (recomendado adicionar guarda `if (!stack || !new_node)`). |
| `stack/free_stack.c` | `free_stack(t_stack *stack)` | Implementado | Liberta todos os nós da pilha. |
| `operations/*.c` | `sa/sb/ss`, `pa/pb`, `ra/rb/rr`, `rra/rrb/rrr` | Implementado | Operações funcionais; alinhar guardas de `NULL` e `size >= 2` em `rr`/`rrr` como em `ss`. |

---

## 4. Checklist de progresso

- [x] Aplicar correção de `convert_numbers()` (`INT_MIN`, `INT_MAX`, overflow, sinais, <= 25 linhas).
- [x] Integrar o parsing no `main` e tratar `argc`/`argv` (argumentos separados e strings com espaços).
- [x] Tratar argumentos vazios/inválidos, duplicados e erros de forma consistente (`Error\n` no `stderr`).
- [x] Criar nós dinamicamente (`new_node`) e definir libertação de memória e limpeza em caso de erro (`free_stack`, `free_split`, `error_exit`).
- [x] Integrar `Makefile`, `Libft`, `push_swap.h` e os novos ficheiros `.c`.
- [ ] Limpar ficheiros temporários (`main_jeanlezy.c` -> `main.c`), remover `<stdio.h>` e comentários `//` de `push_swap.h` para passar 100% na `norminette`.
- [ ] Criar helpers adicionais de stack para ordenação (ex.: `is_sorted`, indexação/rank dos valores).
- [ ] Implementar o cálculo de *disorder* (`compute_disorder`) e as estratégias obrigatórias do enunciado (`--simple`, `--medium`, `--complex`, `--adaptive`).
- [ ] Implementar o modo `--bench` (saída de métricas no `stderr`) e validar metas de operações (100 e 500 números).
- [ ] Testar sequências completas com `checker`, *leaks* (`valgrind`), Norminette final e atualizar o `README.md` de entrega.

---

## 5. Próximos passos imediatos para a dupla

1. **Sincronizar a branch:** garantir que `main.c` na raiz contém o código final de parsing, que `stack/new_node.c` está no `Makefile` e rodar `norminette` em todos os ficheiros.
2. **Helpers de ordenação:** criar função para verificar se a pilha `a` já está ordenada (`is_sorted`) — se já estiver ordenada após o parsing, o programa deve libertar a memória e sair sem imprimir operações.
3. **Métrica de *disorder* e algoritmos:** implementar `compute_disorder` e dividir a implementação das estratégias (`--simple`, `--medium`, `--complex` e `--adaptive`) e da flag `--bench`.