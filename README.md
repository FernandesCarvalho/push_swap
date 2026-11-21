# Push_swap — Estado do projeto

> Documento de acompanhamento para partilhar entre os dois membros da equipa. Atualizado em 21/11/2026 com base no código e nos testes partilhados na conversa. **Não substitui uma revisão do repositório:** ficheiros ou alterações não mostrados aqui devem ser confirmados no código atual.

## 1. Objetivo

Construir o programa `push_swap` em C: receber números inteiros, validá-los, organizar os valores na stack A e produzir no `stdout` uma sequência de instruções (`sa`, `pb`, `ra`, etc.) que os ordene usando as stacks A e B. Quando os valores já estão ordenados, não deve imprimir operações.

## 2. Estrutura de pastas (atual/planeada)

```text
push_swap/
├── Makefile
├── push_swap.h
├── main.c                  # ainda usado para testes temporários
├── libft/                  # biblioteca de apoio
├── parsing/
│   ├── validate_number.c   # implementada
│   └── convert_number.c    # em desenvolvimento
├── stack/
│   └── add_node_back.c     # implementada e testada
├── operations/
│   ├── swap.c              # sa, sb, ss
│   ├── push.c              # pa, pb
│   ├── rotate.c            # ra, rb, rr
│   └── reverse_rotate.c    # rra, rrb, rrr
└── algorithms/             # ainda por implementar
```

Os nomes e a existência física de todos os ficheiros devem ser confirmados no repositório. Em particular, `convert_number.c` foi o nome sugerido para a função `convert_numbers()`.

## 3. Estruturas de dados

Lista duplamente ligada para cada stack:

```c
typedef struct s_node
{
    int             value;
    struct s_node   *prev;
    struct s_node   *next;
} t_node;

typedef struct s_stack
{
    t_node  *head;
    t_node  *tail;
    int     size;
} t_stack;
```

- `head`: primeiro nó (topo).
- `tail`: último nó.
- `size`: número de elementos.
- `prev`/`next`: ligações em ambos os sentidos.

## 4. Funções já trabalhadas

| Área | Função | Estado | Notas |
|---|---|---|---|
| Stack | `add_node_back(t_stack *stack, t_node *new_node)` | Implementada e testada | Acrescenta ao final; atualiza `head`, `tail`, `prev`, `next` e `size`. |
| Operações | `swap(t_stack *stack)` | Implementada e testada | Troca os dois primeiros nós; base para `sa`, `sb`, `ss`. |
| Operações | `sa`, `sb`, `ss` | Implementadas e testadas | `ss` executa ambas as trocas e imprime uma única instrução. |
| Operações | `push(t_stack *dst, t_stack *src)` | Implementada e testada | Move o primeiro nó da origem para o início do destino; ajusta tamanhos e extremos. |
| Operações | `pa`, `pb` | Implementadas e testadas | Assinaturas mantêm sempre a ordem `(a, b)`; `pa` chama `push(a, b)` e `pb` chama `push(b, a)`. |
| Operações | `rotate(t_stack *stack)` | Implementada e testada | Move o primeiro nó para o final; `size` inalterado. |
| Operações | `ra`, `rb`, `rr` | Implementadas e testadas | `rr` roda ambas e imprime uma única instrução. |
| Operações | `reverse_rotate(t_stack *stack)` | Implementada e testada | Move o último nó para o início; `size` inalterado. |
| Operações | `rra`, `rrb`, `rrr` | Implementadas e testadas | `rrr` roda ambas e imprime uma única instrução. |
| Parsing | `validate_number(char *str)` | Implementada; revisão lógica feita | Aceita sinal opcional no início e pelo menos um dígito; rejeita letras, sinais adicionais e string vazia. **Não verifica os limites de `int`.** |
| Parsing | `convert_numbers(char *str, int *result)` | **Incompleta — não corrigida no código local** | Há uma proposta de correção para limites/overflow, mas o utilizador indicou que ainda não a aplicou. |

**Outras funções:** além destas, podem existir helpers no repositório (incluindo funções da Libft). Não foram inventariados nesta conversa; é necessário rever os ficheiros para completar a lista.

## 5. Testes realizados

Foram executados `main.c` temporários com nós construídos manualmente e impressão do conteúdo das stacks:

- `sa`, `sb`, `ss`: resultados de troca confirmados.
- `pa`, `pb`: movimentos confirmados, incluindo esvaziar uma stack e tentar operar com origem vazia.
- `ra`, `rb`, `rr`: rotações confirmadas, incluindo stack vazia e com um nó.
- `rra`, `rrb`, `rrr`: rotações inversas confirmadas, incluindo stack vazia e com um nó.

Os resultados partilhados pelo terminal estavam coerentes com as operações. **Ainda falta** uma verificação sistemática da integridade dos ponteiros (`head`, `tail`, `prev`, `next`) e testes automatizados com sequências de operações.

### Convenções importantes

- Wrappers `pa(a, b)` e `pb(a, b)` recebem sempre `(A, B)`. Só a chamada interna `push(dst, src)` inverte origem/destino quando necessário.
- As operações duplas `ss`, `rr`, `rrr` imprimem **uma** linha, não duas.
- As operações sem efeito devem ser evitadas pelo algoritmo; os wrappers foram ajustados para não imprimir quando nenhuma stack pode executar a operação.
- `write(1, "rra\n", 4)`, `write(1, "rrb\n", 4)` e `write(1, "rrr\n", 4)`: o comprimento inclui `\n`.
- `stdout` do programa final é reservado às instruções; `printf` de diagnóstico pertence apenas aos testes.

## 6. Parsing — estado exato

### `validate_number()`

A implementação discutida:

```c
int validate_number(char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
```

Verifica **formato**, não magnitude nem duplicados. A função pressupõe uma string válida (não `NULL`).

### `convert_numbers()` — pendente

A versão mais recente escrita pelo utilizador ainda:

1. Calculava `res = res * 10 + digit` antes de verificar se o valor ultrapassava o limite, arriscando overflow com strings enormes.
2. Rejeitava incorretamente `-2147483648` por comparar a magnitude positiva com `INT_MAX`.

A solução discutida (ainda **não aplicada**) consiste em:

- `long limit = INT_MAX;` e, para sinal negativo, `limit = -(long)INT_MIN;`.
- Antes de atualizar `res`, calcular `digit = *str - '0'` e rejeitar se `res > (limit - digit) / 10`.
- No fim, guardar `*result = (int)(res * sign)` e devolver `1`; devolver `0` em erro.
- Usar `<limits.h>` para `INT_MIN`/`INT_MAX`.
- Chamar a conversão **apenas depois** de `validate_number()`.

A assinatura acordada é `int convert_numbers(char *str, int *result)`, para distinguir o número válido `0` de uma falha de conversão.

## 7. O que falta fazer

- [ ] Aplicar e testar a correção de `convert_numbers()` (`INT_MIN`, `INT_MAX`, números fora do intervalo, strings gigantes, `0`, sinais).
- [ ] Integrar o parsing no `main` real e tratar `argc`/`argv` (argumentos separados e strings com números separados por espaços, conforme o enunciado).
- [ ] Tratar argumentos vazios/inválidos, duplicados e erros de forma consistente.
- [ ] Criar nós dinamicamente a partir dos números validados; definir libertação de memória e limpeza em caso de erro.
- [ ] Criar/verificar helpers adicionais para stack, conforme necessidade do algoritmo (`new_node`, libertação, deteção de ordenação, etc.).
- [ ] Implementar estratégias de ordenação e escolher a mais adequada conforme os requisitos do enunciado.
- [ ] Integrar o Makefile, Libft, headers e todos os ficheiros `.c`.
- [ ] Testar sequências completas com checker, casos extremos, leaks, Norminette e comportamento de saída.
- [ ] Confirmar no enunciado os requisitos adicionais (flags, benchmarks, algoritmos e eventual bonus) antes de dar o projeto como concluído.

## 8. Próximo passo recomendado

**Terminar `convert_numbers()` e testar o parsing antes de implementar a ordenação.** Depois, implementar a criação/limpeza dos nós e só então começar os algoritmos.

## 9. Nota para colaboração

Este documento descreve **o que foi partilhado e testado durante as sessões**, não é uma auditoria ao estado atual do Git. Antes de dividir tarefas, confirmar os ficheiros reais, protótipos e eventuais funções já criadas fora destas sessões.
