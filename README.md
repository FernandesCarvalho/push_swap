*This project has been created as part of the 42 curriculum by fercarva, jeanbarr.*

# Push_swap — Estado do projeto

**Atualizado: 24/11/2026 | Fase atual: início do Medium (Chunk Sort)**

Documento de desenvolvimento para sincronizar a dupla. Baseado nos ZIPs de 23/11, no README anterior, no código e nos testes partilhados posteriormente e no enunciado Push_swap v1.2. As alterações mais recentes ainda devem ser confirmadas no Git; este documento não certifica a compilação ou os testes do repositório atual.

## Description — Objetivo

Ordenar números inteiros em ordem crescente na stack A, utilizando uma stack auxiliar B e apenas as 11 operações permitidas. O programa deve minimizar as operações emitidas, implementar quatro estratégias e selecionar a estratégia adequada em tempo de execução.

## 1. Estado geral

| Etapa | Estado |
|---|---|
| Estruturas, Libft e Makefile | Implementados; revisão final pendente |
| Parsing e tratamento de erros | Implementados; testes manuais realizados |
| 11 operações | Implementadas; testes manuais realizados; revisão das operações duplas pendente |
| Ordenação de 2 a 5 elementos | Implementada e testada manualmente |
| Simple — O(n²) | Implementado e testado manualmente |
| Indexação | Implementada; teste confirmado |
| `ft_sqrt()` | Versão final definida; confirmar integração e testar |
| Medium — O(n√n) | Em início de desenvolvimento |
| Complex — O(n log n) | Planeado |
| Disorder e Adaptive | Pendentes |
| Flags e benchmark | Pendentes |
| Testes finais, Norminette e memória | Pendentes |
| Checker próprio (bonus) | Pendente; opcional |

## 2. Estrutura atual de trabalho

```text
push_swap/
├── Makefile
├── push_swap.h
├── main.c                  # testes antigos
├── main_jeanlezy.c         # main utilizado atualmente
├── libft/
├── parsing/
│   ├── validate_number.c
│   ├── convert_numbers.c
│   ├── has_duplicate.c
│   ├── error_exit.c
│   └── parse_args.c
├── stack/
│   ├── new_node.c
│   ├── add_node_back.c
│   └── free_stack.c
├── operations/
│   ├── swap.c
│   ├── push.c
│   ├── rotate.c
│   └── reverse_rotate.c
└── sorting/
    ├── sort_small.c
    ├── sort_utils.c
    ├── sort_simple.c
    ├── indexing.c
    ├── sort_medium.c       # a desenvolver
    ├── sort_complex.c      # planeado
    └── sort_adaptive.c     # planeado
```

Confirmar a remoção de `sorting/is_sorted.c` da compilação, pois `is_sorted()` passou para `sort_utils.c`. Confirmar os nomes e protótipos após as alterações recentes.

## 3. Estruturas de dados

Lista duplamente ligada. Estrutura pretendida após a indexação:

```c
typedef struct s_node
{
    int             value;
    int             index;
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

`value` guarda o inteiro original; `index` representa a posição que o valor teria na lista ordenada (0 = menor). `head` é o topo, `tail` é o fundo e `size` é o número de nós. Confirmar a inicialização de `index` em `new_node()`; recomenda-se `-1` antes da indexação. As operações movem nós, preservando a associação entre valor e índice.

## 4. Funções implementadas

### Parsing e gestão de memória

| Função | Responsabilidade |
|---|---|
| `validate_number()` | Validar o formato numérico; helper anterior, confirmar se ainda é utilizado |
| `convert_numbers()` | Validar e converter para int, incluindo limites e proteção de overflow |
| `has_duplicate()` | Detetar números repetidos |
| `parse_args()` | Percorrer argumentos, separar por espaços, validar e preencher A |
| `append_to_a()` / `process_token()` | Helpers internos do parsing |
| `free_split()` / `error_exit()` | Limpar memória e emitir `Error\n` no stderr em caso de erro |
| `new_node()` | Criar nó dinamicamente |
| `add_node_back()` | Adicionar nó ao fundo da stack |
| `free_stack()` | Libertar os nós da stack |
| `init_stack()` | Inicializar head, tail e size no main |

### Operações

| Operação | Efeito |
|---|---|
| `sa`, `sb`, `ss` | Trocar os dois primeiros elementos de A, B ou ambas |
| `pa`, `pb` | Transferir o topo de B para A ou de A para B |
| `ra`, `rb`, `rr` | Mover o primeiro elemento para o fundo |
| `rra`, `rrb`, `rrr` | Mover o último elemento para o topo |

Helpers internos: `swap()`, `push()`, `rotate()`, `reverse_rotate()`. Os wrappers `pa(a,b)` e `pb(a,b)` mantêm a ordem dos argumentos A, B. Cada operação dupla deve emitir uma única instrução.

**Revisão necessária:** no ZIP anterior, `ss`, `rr` e `rrr` retornavam quando qualquer stack tinha menos de dois elementos. Confirmar e corrigir a semântica: cada lado deve poder executar a sua parte independentemente, mesmo quando o outro não pode. Não confundir implementação existente com conformidade final.

### Ordenação de stacks pequenas — `sort_small.c`

- `sort_two()`: compara os dois valores e executa `sa` se necessário.
- `sort_three()`: coloca o maior no fundo com `ra` ou `rra`, depois corrige os dois primeiros com `sa` se necessário.
- `sort_four()`: envia o menor para B, ordena os três restantes e recupera o menor.
- `sort_five()`: envia os dois menores para B, ordena os três restantes e recupera ambos.

### Helpers — `sort_utils.c`

- `is_sorted()`: verifica a ordem crescente, sem modificar a stack.
- `find_min_pos()`: percorre a stack e encontra a posição física do menor valor.
- `push_min_to_b()`: anteriormente `push_to_b()`; usa `ra` ou `rra` pelo caminho mais curto até ao menor e executa `pb`.
- `ft_sqrt()`: calcula a raiz quadrada inteira, arredondada para baixo, para definir o tamanho dos chunks.

## 5. Simple — `sort_simple.c`

Estratégia de extração sucessiva do mínimo (adaptação de Selection Sort):

1. Enquanto A tiver mais de dois elementos, mover o menor para B.
2. Ordenar os dois restantes com `sort_two()`.
3. Recuperar todos os elementos de B com `pa`.

Como os mínimos são enviados por ordem crescente, B fica com os maiores desses mínimos no topo. Ao recuperá-los, A fica ordenada. Cada extração pode exigir O(n) rotações; repetida O(n) vezes, resulta em O(n²) operações no pior caso. Memória auxiliar do algoritmo: O(1), além dos nós das duas stacks (O(n)).

A versão atual deixa dois elementos por decisão de implementação. Deixar três é uma possível otimização futura.

## 6. Indexação — `indexing.c`

`assign_index()` atribui a cada nó o número de valores inferiores ao seu próprio valor. Não ordena nem modifica fisicamente a stack.

```c
void assign_index(t_stack *a)
{
    int     index;
    t_node  *check;
    t_node  *current;

    if (!a || a->size < 1)
        return ;
    current = a->head;
    while (current)
    {
        check = a->head;
        index = 0;
        while (check)
        {
            if (current->value > check->value)
                index++;
            check = check->next;
        }
        current->index = index;
        current = current->next;
    }
}
```

Teste confirmado no terminal:

```text
Value: 42 | Index: 3
Value: -15 | Index: 0
Value: 800 | Index: 4
Value: 0 | Index: 1
Value: 7 | Index: 2
ra
pb
ra
pb
pa
pa
```

Complexidade computacional O(n²), memória auxiliar O(1), zero operações Push_swap. O enunciado (§VI.3.1) mede as classes obrigatórias pelo número de operações geradas, portanto não é obrigatório substituir esta indexação por Merge Sort ou pesquisa binária para cumprir esse modelo. Uma otimização de CPU pode ser feita posteriormente.

## 7. Nova função `ft_sqrt()`

Versão final proposta para `sort_utils.c`:

```c
int ft_sqrt(int n)
{
    int i;

    if (n < 1)
        return (0);
    i = 1;
    while (i <= n / i)
        i++;
    return (i - 1);
}
```

Calcula o maior inteiro cujo quadrado não ultrapassa n. Utiliza divisão para evitar overflow da multiplicação. Tempo computacional O(√n), memória O(1), zero operações Push_swap.

| n | Resultado esperado |
|---:|---:|
| 0 | 0 |
| 1 | 1 |
| 15 | 3 |
| 16 | 4 |
| 100 | 10 |
| 500 | 22 |
| INT_MAX (32 bits) | 46340 |

Adicionar `int ft_sqrt(int n);` ao header e confirmar os testes. Não foi apresentado ainda o resultado da compilação desta versão.

## 8. Fase atual — Medium (Chunk Sort)

Objetivo: O(n√n) operações. Usar os índices para dividir os elementos em intervalos de tamanho aproximadamente √n.

Exemplo: 16 elementos, chunks de tamanho 4:

| Chunk | Índices |
|---|---|
| 1 | 0–3 |
| 2 | 4–7 |
| 3 | 8–11 |
| 4 | 12–15 |

Plano inicial:

1. Calcular `chunk_size = ft_sqrt(a->size)` e garantir tamanho positivo para entrada não vazia.
2. Percorrer A e enviar para B os elementos do intervalo atual.
3. Repetir para os restantes intervalos, incluindo o último incompleto.
4. Recuperar elementos de B por índice decrescente, escolhendo o caminho mais curto.
5. Verificar que A está ordenada e B vazia.

**Atenção à complexidade:** enviar os chunks para B com um percurso por intervalo pode custar O(n√n) operações. Porém, procurar e rodar até ao máximo em toda B repetidamente, sem controlar a disposição dos elementos, pode custar O(n²) operações. Precisamos de manter os chunks organizados em B (por exemplo, blocos contíguos por intervalo) e limitar as rotações durante a recuperação. A estratégia final deve ter uma justificação do limite O(n√n); ainda não está implementada nem demonstrada.

Próximas tarefas imediatas:

- [ ] Confirmar `ft_sqrt()` e protótipo.
- [ ] Remover impressões temporárias de índices do main.
- [ ] Criar `sort_medium.c` e atualizar header/Makefile.
- [ ] Implementar transferência por intervalos e organização de B.
- [ ] Implementar recuperação por máximo com limite de rotações por chunk.
- [ ] Integrar temporariamente o Medium no main e testar.
- [ ] Medir operações e justificar a complexidade.

## 9. Complex e Adaptive — planeados

**Complex (Radix Sort binário):** trabalhar com os índices não negativos; para cada bit, distribuir os nós entre A e B com `pb` e `ra`, recuperar B com `pa` e repetir até processar todos os bits. O(n log n) operações, pois existem O(log n) bits e O(n) operações por passagem. Memória auxiliar O(1), além das stacks. Ainda não implementado.

**Disorder:** obrigatório antes de qualquer movimento. Contar pares invertidos (valor maior antes de menor) e dividir pelo número total de pares. Para menos de dois elementos, devolver zero, evitando divisão por zero.

| Disorder | Regime Adaptive | Método planeado |
|---|---|---|
| < 0.2 | O(n²) | Simple ou adaptação para baixa desordem |
| 0.2 ≤ d < 0.5 | O(n√n) | Medium |
| ≥ 0.5 | O(n log n) | Complex |

Os limiares são definidos pelo enunciado. Falta implementar a seleção, justificar as técnicas escolhidas para cada regime e medir o desempenho. Adaptive será a estratégia por defeito.

## 10. Main e integração

O main atual inicializa A/B, executa `parse_args()`, termina sem operações se A estiver ordenada, seleciona `sort_two/three/four/five` para tamanhos pequenos e `sort_simple()` para tamanhos superiores a cinco, e liberta ambas as stacks.

Ainda falta integrar os seletores de estratégia, medir disorder antes dos movimentos e garantir que cada seletor funciona independentemente do tamanho ou desordem. A indexação deve ocorrer antes das estratégias que dela necessitam. Rever a divisão do main em helpers para a Norminette após a implementação funcional.

## Instructions — Compilação e execução

```sh
make
./push_swap 42 -15 800 0 7
./push_swap "5 4 3 2 1"
./push_swap 1 2 3 4 5
make re
```

A última entrada ordenada não deve emitir operações. Sem argumentos, o programa também deve permanecer silencioso. Erros devem emitir `Error\n` em stderr. O stdout final deve conter exclusivamente operações, uma por linha.

Quando as flags estiverem implementadas:

```sh
./push_swap --simple 5 4 3 2 1
./push_swap --medium 42 -15 800 0 7
./push_swap --complex 42 -15 800 0 7
./push_swap --adaptive --bench 42 -15 800 0 7
```

Estes últimos comandos são exemplos futuros, não funcionalidades disponíveis atualmente.

## 11. Testes e desempenho

Testes manuais já realizados: operações individuais, parsing de argumentos separados e agrupados, negativos, duplicados, valores fora do intervalo, ausência de argumentos, ordenação pequena, Simple com entradas de 6 e 10 elementos e indexação com cinco valores. Não equivalem a uma validação exaustiva.

Metas do enunciado (número de operações):

| Entrada aleatória | Mínimo para passar | Bom | Excelente |
|---:|---:|---:|---:|
| 100 | < 2000 | < 1500 | < 700 |
| 500 | < 12000 | < 8000 | < 5500 |

```sh
ARG=$(shuf -i 0-9999 -n 100)
./push_swap $ARG | wc -l
# Quando o checker fornecido estiver disponível:
./push_swap $ARG | ./checker_linux $ARG
```

Falta testar múltiplas entradas aleatórias, todas as permutações pequenas, extremos INT_MIN/INT_MAX, erros, invariantes dos ponteiros, leaks e comportamento de cada estratégia. A contagem de operações não comprova por si só que a ordenação está correta.

## 12. Checklist até à entrega

- [ ] Concluir Medium e demonstrar O(n√n) operações.
- [ ] Implementar Complex e demonstrar O(n log n) operações.
- [ ] Implementar `compute_disorder()` e Adaptive.
- [ ] Implementar `--simple`, `--medium`, `--complex`, `--adaptive` (default).
- [ ] Implementar `--bench`: disorder em percentagem com duas casas decimais, estratégia/classe, total e contagem das 11 operações; saída apenas em stderr quando solicitado.
- [ ] Rever operações duplas e casos sem efeito.
- [ ] Confirmar limites de desempenho para 100 e 500 números.
- [ ] Testar memória, ponteiros e erros de alocação.
- [ ] Executar Norminette, rever tamanho do main, comentários e Makefile sem relink.
- [ ] Remover código de diagnóstico e ficheiros de teste desnecessários da entrega.
- [ ] Rever README final, justificar algoritmos e confirmar contribuições dos dois membros.
- [ ] Opcional: implementar checker próprio após validar a parte obrigatória.

## 13. Contribuições da dupla

Registo preliminar, baseado no histórico e nos cabeçalhos dos ficheiros partilhados; ambos devem confirmar e completar antes da entrega.

- **jeanbarr:** modularização do parsing, integração inicial do main, criação inicial de `is_sorted()` e trabalho de base na gestão das stacks.
- **fercarva:** desenvolvimento e testes das operações, ordenação pequena, helpers de mínimo, Simple, indexação e preparação do Medium.
- **Ambos:** revisão cruzada, integração, testes, decisões de algoritmos e preparação da defesa. Cada membro deve conseguir explicar todas as partes do projeto.

## Resources — Referências e utilização de IA

- Enunciado oficial Push_swap, versão 1.2, especialmente §VI.3, §VI.5, §VI.6 e capítulo VII.
- Manual de C e páginas locais `man 3 malloc`, `man 3 free`, `man 2 write`.
- Thomas H. Cormen et al., *Introduction to Algorithms* — referência para análise de complexidade e algoritmos de ordenação (consulta recomendada).
- ChatGPT: utilizado para discutir algoritmos, orientar a implementação incremental, rever lógica, simular exemplos, sugerir testes, propor a versão final de `ft_sqrt()` e atualizar a documentação. Os membros devem compreender, rever e testar as sugestões antes de as integrar.

**Próximo objetivo da dupla:** terminar o Chunk Sort, garantindo correção e complexidade no modelo de operações do Push_swap, antes de avançar para Radix e Adaptive.
