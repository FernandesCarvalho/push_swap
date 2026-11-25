*This project has been created as part of the 42 curriculum by fercarva, jeanbarr.*

# Push_swap — Estado do projeto

**Atualizado: 24/11/2026 | Fase atual: Medium validado + Parsing de flags integrado (próximo: Complex, Disorder e Benchmark)**

Documento de desenvolvimento para sincronizar a dupla. Baseado no README anterior, nos ficheiros atuais do repositório (`Makefile`, `push_swap.h`, `main_jeanlezy.c`, `parsing/`, `sorting/`, `stack/`, `operations/`) e nos testes executados com `./checker_linux` e `valgrind`.

## Description — Objetivo

Ordenar números inteiros em ordem crescente na stack A, utilizando uma stack auxiliar B e apenas as 11 operações permitidas. O programa deve minimizar as operações emitidas, implementar quatro estratégias (`simple`, `medium`, `complex` e `adaptive`) e selecionar a estratégia adequada em tempo de execução.

## 1. Estado geral

| Etapa | Estado |
|---|---|
| Estruturas, Libft e Makefile | Implementados e compilando sem avisos (`-Wall -Wextra -Werror`) |
| Parsing de argumentos numéricos e tratamento de erros | Implementados e validados (`parse_args` recebe `start`) |
| Parsing de flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`) | Implementado (`parse_flags.c` + `t_config`) e testado |
| 11 operações | Implementadas; testes realizados; revisão final das operações duplas pendente |
| Ordenação de 2 a 5 elementos | Implementada e validada (`sort_small.c`) |
| Simple — $O(n^2)$ | Implementado e validado (`sort_simple.c`) |
| Indexação e `ft_sqrt()` | Implementados e validados (`indexing.c`, `sort_utils.c`) |
| Medium — $O(n\sqrt{n})$ (Chunk Sort) | **Implementado e validado** (`sort_medium.c`, `chunk_utils.c`) — 20/20 testes `OK` para 100 e 500 números |
| Complex — $O(n \log n)$ | Planeado (próxima etapa) |
| Disorder (`compute_disorder`) e regime Adaptive | Pendentes (atualmente `--adaptive` encaminha para o fluxo existente) |
| Saída do `--bench` (contagem das 11 operações no `stderr`) | Pendente (`config.bench` já é lido e armazenado) |
| Testes finais, Norminette e memória | Em andamento (`valgrind` já validado com 0 leaks no fluxo atual) |
| Checker próprio (bonus) | Pendente; opcional |

## 2. Estrutura atual de trabalho

```text
push_swap/
├── Makefile
├── push_swap.h
├── checker_linux           # binário de validação local (não entregar)
├── main_jeanlezy.c         # main utilizado atualmente (renomear/limpar na entrega)
├── libft/
├── parsing/
│   ├── convert_numbers.c
│   ├── has_duplicate.c
│   ├── error_exit.c
│   ├── parse_args.c
│   └── parse_flags.c
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
    ├── chunk_utils.c
    ├── sort_medium.c
    ├── sort_complex.c      # planeado
    └── sort_adaptive.c     # planeado
```

*Nota:* `parsing/validate_number.c` foi removido da compilação e do repositório por já estar coberto por `convert_numbers.c` (confirmar remoção do protótipo residual em `push_swap.h` na limpeza final).

## 3. Estruturas de dados

### Pilhas e nós (`t_node`, `t_stack`)

Lista duplamente ligada com ponteiros para topo (`head`), fundo (`tail`) e contador (`size`):

```c
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*prev;
	struct s_node	*next;
}	t_node;

typedef struct s_stack
{
	t_node	*head;
	t_node	*tail;
	int		size;
}	t_stack;
```

### Chunks e configuração de execução (`t_chunk`, `t_strategy`, `t_config`)

```c
typedef struct s_chunk
{
	int	start;
	int	end;
	int	mid;
}	t_chunk;

typedef enum e_strategy
{
	STRAT_ADAPTIVE,
	STRAT_SIMPLE,
	STRAT_MEDIUM,
	STRAT_COMPLEX
}	t_strategy;

typedef struct s_config
{
	t_strategy	strategy;
	int			bench;
	int			arg_start;
}	t_config;
```

`t_config` armazena a estratégia escolhida (padrão `STRAT_ADAPTIVE`), se `--bench` está ativo (`0` ou `1`) e `arg_start`, o índice em `argv` onde terminam as flags e começam os números.

## 4. Funções implementadas

### Parsing e gestão de memória

| Ficheiro / Função | Responsabilidade |
|---|---|
| `parse_flags.c` (`init_config`, `set_bench`, `set_strategy`, `parse_flags`, `same_str`) | Lê flags iniciais (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`) usando `ft_strncmp` e define `config->arg_start` |
| `parse_args.c` (`parse_args`, `process_token`, `append_to_a`) | Percorre `argv` a partir de `start`, separa tokens por `' '`, valida e preenche a stack A |
| `convert_numbers.c` / `has_duplicate.c` | Valida formato numérico, limites `INT_MIN`/`INT_MAX`, overflow e duplicados |
| `error_exit.c` / `free_stack()` | Liberta memória (`split` e pilhas) e emite `Error\n` no `stderr` |
| `new_node()` / `add_node_back()` | Criação e inserção de nós na lista duplamente ligada |

### Operações

| Operação | Efeito |
|---|---|
| `sa`, `sb`, `ss` | Trocar os dois primeiros elementos de A, B ou ambas |
| `pa`, `pb` | Transferir o topo de B para A ou de A para B |
| `ra`, `rb`, `rr` | Mover o primeiro elemento para o fundo |
| `rra`, `rrb`, `rrr` | Mover o último elemento para o topo |

**Revisão pendente:** confirmar na revisão final se `ss`, `rr` e `rrr` executam cada lado de forma independente quando apenas uma das stacks tem tamanho $\ge 2$.

### Ordenação pequena e utilitários (`sort_small.c`, `sort_utils.c`, `indexing.c`)

- `sort_two()`, `sort_three()`, `sort_four()`, `sort_five()`: ordenação dedicada para entradas de 2 a 5 elementos.
- `is_sorted()`: verifica ordem crescente sem modificar a stack.
- `find_min_pos()` / `push_min_to_b()`: localiza o menor valor e envia para B pelo caminho mais curto (`ra` ou `rra`).
- `ft_sqrt()`: calcula $\lfloor \sqrt{n} \rfloor$ sem risco de overflow (`i <= n / i`).
- `assign_index()`: atribui a cada nó a sua posição ordenada (`0` a `n - 1`) sem alterar a ordem física da stack.

## 5. Simple — `sort_simple.c`

Adaptação de *Selection Sort* para duas stacks ($O(n^2)$ operações, $O(1)$ memória auxiliar):
1. Enquanto A tiver mais de 2 elementos, envia o menor para B (`push_min_to_b`).
2. Ordena os 2 restantes em A com `sort_two()`.
3. Devolve todos os elementos de B para A com `pa`.

## 6. Medium — `sort_medium.c` e `chunk_utils.c` (Validado)

Estratégia baseada em *Chunk Sort* sobre os índices normalizados ($O(n\sqrt{n})$):

1. `assign_index(a)` calcula o índice final (`0` a `n - 1`) de cada nó.
2. `push_chunks_to_b()` define `chunk_size = ft_sqrt(total)` e percorre A enviando para B os elementos do intervalo atual `[chunk.start, chunk.end)`:
   - Elementos da metade inferior do chunk (`index < chunk.mid`) recebem `rb(b)` após `pb(a, b)`, pré-organizando B em formato ampulheta (maiores nas extremidades, menores no meio).
   - Ao completar o chunk (`pushed == target`), `next_chunk()` avança para o intervalo seguinte, ajustando o último chunk caso `total` não seja múltiplo exato de `chunk_size`.
3. `push_back_to_a()` chama `push_max_to_a()` enquanto B não estiver vazia, localizando o maior índice (`find_max_pos`) e rodando B pelo caminho mais curto (`rb` se `pos <= b->size / 2`, caso contrário `rrb`) antes de executar `pa(a, b)`.

### Resultados medidos no Medium (`./checker_linux`)

- **100 números (20 rodadas aleatórias):** **20/20 `OK`**, entre **759 e 824 operações** (meta mínima: `< 2000`; meta "Bom": `< 1500`).
- **500 números (20 rodadas aleatórias):** **20/20 `OK`**, entre **7357 e 7680 operações** (meta mínima: `< 12000`; meta "Bom": `< 8000`).

## 7. Próximas etapas: Complex, Disorder e Benchmark

- **Complex (`sort_complex.c` — $O(n \log n)$):** implementar *Radix Sort* binário sobre os índices (`0` a `n - 1`), processando bit a bit com `pb`/`ra` e recuperando B com `pa`.
- **Disorder (`compute_disorder`):** calcular antes de qualquer movimento a proporção de pares invertidos em A ($\text{inversões} / \text{total de pares}$, retornando `0.0` se `size < 2`).
- **Adaptive (`STRAT_ADAPTIVE`):** despachar conforme o *disorder*:
  - `< 0.2`: estratégia de baixa desordem / $O(n^2)$
  - $0.2 \le d < 0.5$: Medium ($O(n\sqrt{n})$)
  - $\ge 0.5$: Complex ($O(n \log n)$)
- **Benchmark (`--bench`):** registar a contagem individual das 11 operações e, quando `config.bench == 1`, imprimir no `stderr` a percentagem de *disorder* (2 casas decimais), a estratégia utilizada, o total de operações e o detalhe de cada instrução.

## Instructions — Compilação e execução

```sh
make
./push_swap 42 -15 800 0 7
./push_swap "5 4 3 2 1"
./push_swap 1 2 3 4 5
./push_swap --simple 5 4 3 2 1
./push_swap --medium 42 -15 800 0 7
./push_swap --complex 42 -15 800 0 7
./push_swap --adaptive --bench 42 -15 800 0 7
```

- Entradas vazias, apenas com flags ou já ordenadas terminam com status `0` sem emitir operações.
- Flags desconhecidas (ex.: `--invalida`) ou argumentos inválidos/duplicados emitem `Error\n` no `stderr` e terminam com status `1`.

## 8. Testes já realizados

- **Flags e casos de borda:** `./push_swap`, `./push_swap --bench`, `./push_swap --simple --bench`, entradas já ordenadas, `--simple`, `--medium --bench`, `--bench --medium`, `--complex`, `--adaptive` e flags inválidas (`--invalida` retornando `Error` e status `1`).
- **Corretude (`./checker_linux`):** `OK` em todas as baterias de 100 e 500 números no Medium e nos testes do Simple e casos pequenos.
- **Memória (`valgrind --leak-check=full`):** `24 allocs, 24 frees`, `0 bytes in 0 blocks` e `0 errors`.

## 9. Checklist até à entrega

- [x] Implementar e validar Medium (`sort_medium.c`, `chunk_utils.c`) com 100 e 500 números.
- [x] Implementar parsing de flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`) em `parse_flags.c`.
- [ ] Implementar Complex ($O(n \log n)$) e demonstrar operações para 100 e 500 números.
- [ ] Implementar `compute_disorder()` e seleção automática no modo Adaptive.
- [ ] Implementar saída completa do `--bench` no `stderr` (percentagem de disorder, estratégia, total e contagem das 11 operações).
- [ ] Rever operações duplas (`ss`, `rr`, `rrr`) e casos sem efeito.
- [ ] Executar `norminette` em todos os `.c` e `.h` (remover comentários `//`, ajustar espaçamento e cabeçalhos).
- [ ] Limpar ficheiros temporários (`checker_linux`, `main_jeanlezy.c` $\rightarrow$ `main.c`, protótipos não utilizados) e confirmar `Makefile` sem relink.
- [ ] Revisão final da documentação e defesa da dupla.

## 10. Contribuições da dupla

- **jeanbarr:** modularização do parsing (`parse_args.c`, `parse_flags.c`), estruturas de configuração (`t_config`), integração do `main`, desenvolvimento e validação do Medium (`chunk_utils.c`, `sort_medium.c`), `Makefile` e bateria de testes.
- **fercarva:** desenvolvimento das operações, ordenação de stacks pequenas (`sort_small.c`), utilitários (`sort_utils.c`), Simple (`sort_simple.c`), indexação (`indexing.c`) e base inicial do Medium.
- **Ambos:** revisão cruzada, integração, testes com `checker_linux` e `valgrind`, e preparação da defesa.

## Resources — Referências e utilização de IA

- Enunciado oficial *Push_swap*, versão 1.2 (§VI.3, §VI.5, §VI.6 e capítulo VII).
- Manual de C e páginas locais `man 3 malloc`, `man 3 free`, `man 2 write`.
- Thomas H. Cormen et al., *Introduction to Algorithms* — referência para análise de complexidade e algoritmos de ordenação.
- Assistentes de IA (ChatGPT / Gemini): utilizados para discutir algoritmos, estruturar a modularização do parsing e do Chunk Sort, depurar assinaturas/compilação, gerar scripts de teste e atualizar a documentação do projeto.