# Push_swap — Diário de Desenvolvimento do Projeto

> Documento de acompanhamento atualizado em **22/11/2026** para sincronizar o estado atual da branch (`jeanlezy-dev`) entre os membros da dupla.

---

## 1. Diário de Bordo

### 📅 22/11/2026 — Modularização do Parsing, Helpers de Ordenação e Integração no `main`

1. **Extração e modularização do *parsing* (`parsing/parse_args.c`):**
   * Movida a lógica principal de leitura de `argc`/`argv` do `main` para `parsing/parse_args.c`, dividida em `append_to_a()`, `process_token()` e `parse_args()`.
   * Simplificado o fluxo de validação em `process_token()` para usar diretamente `convert_numbers()` (que já valida sinais, formato numérico e limites `INT_MIN`/`INT_MAX`), eliminando a redundância com `validate_number()`.
   * Corrigidos detalhes críticos no `parse_args.c` (remoção de `;` acidental após `if (process_token(...))`, substituição de `free(split)` por `free_split(split)` para evitar *leaks* e remoção de comentários internos para conformidade com a **Norminette**).
   * Renomeado `parsing/error.c` para `parsing/error_exit.c` para refletir o nome da função principal.

2. **Alinhamento final nas Operações (`operations/`):**
   * Padronizadas as verificações de ponteiros nulos e tamanho mínimo (`size >= 2`) nas operações duplas (`ss`, `rr`, `rrr`) em `swap.c`, `rotate.c` e `reverse_rotate.c`.

3. **Criação da pasta `sorting/` e helper `is_sorted()`:**
   * Criado `sorting/is_sorted.c` com a função `is_sorted(t_stack *stack)` para verificar se a pilha `a` já se encontra ordenada em ordem crescente.
   * Adicionado o protótipo `int is_sorted(t_stack *stack);` em `push_swap.h`.

4. **Atualização do `main.c`:**
   * Conectados `parse_args()` e `is_sorted(&a)` no fluxo principal: se a pilha já estiver ordenada após o *parsing*, o programa liberta `a` e `b` com `free_stack()` e encerra com `return (0)` sem imprimir nenhuma operação.

---

### 📅 21/11/2026 — Resumo da Etapa Anterior (Base, Parsing Unitário e Operações)

* **Infraestrutura:** configurado o `Makefile` na raiz com `-Wall -Wextra -Werror` e integração da `libft`, além das declarações iniciais em `push_swap.h`.
* **Validação e Erros (`parsing/`):** finalizado `convert_numbers.c` com proteção de *overflow*/*underflow* (`check_overflow`), criado `has_duplicate.c` para rejeitar repetidos e implementadas `free_split()` e `error_exit()` (`Error\n` no `fd 2`).
* **Gestão de Nós (`stack/`):** implementadas `new_node.c`, `add_node_back.c` e `free_stack.c` para manipulação e limpeza da lista duplamente ligada.
* **Operações (`operations/`):** implementadas e protegidas contra `NULL` as operações `sa/sb/ss`, `pa/pb`, `ra/rb/rr` e `rra/rrb/rrr`.

---

## 2. Estrutura atual de pastas e ficheiros

```text
42-push_swap/
├── Makefile
├── README.md
├── push_swap.h
├── main.c                      # Fluxo principal (init_stack, parse_args, is_sorted, free_stack)
├── main_jeanlezy.c             # Ficheiro auxiliar de transição/testes (remover após sync)
├── libft/                      # Biblioteca de apoio
├── parsing/
│   ├── parse_args.c            # append_to_a(), process_token(), parse_args()
│   ├── convert_numbers.c       # check_overflow(), convert_numbers()
│   ├── has_duplicate.c         # has_duplicate()
│   ├── error_exit.c            # free_split(), error_exit()
│   └── validate_number.c       # (obsoleto — substituído por convert_numbers)
├── sorting/
│   └── is_sorted.c             # is_sorted()
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

## 3. Tabela de ficheiros e estado atual

| Pasta / Ficheiro | Funções | Estado | Observações |
|---|---|---|---|
| `Makefile` | Regras `all`, `$(NAME)`, `clean`, `fclean`, `re` | Atualizado | Garantir inclusão de `parsing/parse_args.c`, `parsing/error_exit.c` e `sorting/is_sorted.c` em `SRCS`. |
| `push_swap.h` | Structs e protótipos globais | Atualizado | Inclui protótipo de `is_sorted()`; adicionar `parse_args()` e limpar `validate_number()` se removido. |
| `main.c` | `init_stack`, `main` | Atualizado | Executa *parsing*, verifica `is_sorted(&a)` e liberta memória antes de sair. |
| `parsing/parse_args.c` | `append_to_a`, `process_token`, `parse_args` | Implementado | Faz split por espaços, valida, converte, checa duplicados e monta a pilha `a`. |
| `parsing/convert_numbers.c` | `check_overflow`, `convert_numbers` | Pronto | Valida formato numérico e limites `INT_MIN` / `INT_MAX` (<= 25 linhas). |
| `parsing/has_duplicate.c` | `has_duplicate(t_stack *a, int value)` | Pronto | Rejeita valores duplicados antes da inserção. |
| `parsing/error_exit.c` | `free_split`, `error_exit` | Pronto | Liberta `split`, `a` e `b`, escreve `Error\n` no `fd 2` e devolve `1`. |
| `sorting/is_sorted.c` | `is_sorted(t_stack *stack)` | Implementado | Verifica se a pilha está em ordem crescente (`1` se ordenada, `0` caso contrário). |
| `stack/*.c` | `new_node`, `add_node_back`, `free_stack` | Pronto | Alocação, inserção e libertação completa dos nós. |
| `operations/*.c` | `sa/sb/ss`, `pa/pb`, `ra/rb/rr`, `rra/rrb/rrr` | Pronto | Todas as operações protegidas contra `NULL` e `size < 2`. |

---

## 4. Checklist de progresso

- [x] Implementar e validar `convert_numbers()`, `has_duplicate()` e `error_exit()`.
- [x] Modularizar o *parsing* em `parsing/parse_args.c` com limpeza segura (`free_split`).
- [x] Padronizar proteções de `NULL` e `size` em todas as operações (`operations/`).
- [x] Criar `sorting/is_sorted.c` e integrar a saída antecipada sem operações no `main.c`.
- [x] Remover ficheiros obsoletos/temporários (`parsing/validate_number.c` e `main_jeanlezy.c`) e atualizar `Makefile` / `push_swap.h`.
- [x] Validar compilação (`make re`), `valgrind` (0 *leaks* em casos válidos e de erro) em todo o projeto.
- [ ] Criar helpers adicionais de *stack* para ordenação (ex.: indexação/rank dos valores, encontrar mínimo/máximo).
- [ ] Implementar ordenação para casos pequenos (2 a 5 elementos) e o algoritmo principal (100 e 500 números) / cálculo de *disorder* e flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`), conforme o *subject*.
- [ ] Testar sequências completas com `checker`, *leaks* (`valgrind`), Norminette final e atualizar o `README.md` de entrega.

---

## 5. Próximos passos imediatos para a dupla

1. **Indexação e casos pequenos:** implementar a atribuição de índices relativos (`0` a `n - 1`) na pilha `a` e a ordenação direta para pilhas pequenas (2, 3 e até 5 elementos).
2. **Estratégia principal de ordenação:** implementar o algoritmo para pilhas maiores (100 e 500 elementos) respeitando as metas de operações do enunciado.