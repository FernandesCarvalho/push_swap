*This project has been created as part of the 42 curriculum by fercarva, jeanbarr.*

# Push_swap — Diário de Bordo e Estado do Projeto

**Atualizado: 25/11/2026 | Fase atual: Benchmark (`--bench`), Adaptive e testes de 100/500 validados — reta final de Norminette, limpeza e entrega**

Diário de bordo e registo técnico para sincronizar a dupla (`jeanbarr` e `fercarva`) a partir da base anterior de 24/11/2026, refletindo o que foi alterado e testado no dia 25/11/2026 com `norminette`, `valgrind` e `ft_ps_tester`.

## Description — Objetivo

Ordenar números inteiros em ordem crescente na stack A, utilizando uma stack auxiliar B e apenas as 11 operações permitidas. O programa deve minimizar as operações emitidas, implementar quatro estratégias (`simple`, `medium`, `complex` e `adaptive`) e selecionar a estratégia adequada em tempo de execução.

## Diário de Bordo — O que rolou no dia 25/11/2026

* **Refatoração do `parse_args` (`push_swap.h`, `parsing/parse_args.c`, `main.c`):** reduzimos o protótipo de 5 para 4 argumentos (`int parse_args(int argc, char **argv, t_stack *a, t_stack *b)`), passando `argc - config.arg_start` e `argv + config.arg_start` a partir do `main` para cumprir o limite de argumentos da Norminette.
* **Relatório `--bench` no `stderr` (`parsing/bench.c`):** implementamos a saída completa no `fd 2` (`Disorder: <pct>%`, nome da estratégia/complexidade — incluindo prefixo `Adaptive -> ` quando `cfg->strategy == STRAT_ADAPTIVE` —, `Total ops` e a contagem individual das 11 operações).
* **Integração no `main.c` e cálculo de `disorder`:** passamos a calcular `disorder = compute_disorder(&a)` fora do bloco exclusivo do modo adaptativo, resolver `used_strat` com `resolve_strategy(&config, disorder)` e chamar `print_bench(&config, used_strat, disorder)` antes de libertar as pilhas.
* **Ajuste do limiar no `choose_adaptive.c`:** reduzimos o corte do `STRAT_SIMPLE` de `0.2` para `0.05` para impedir que pilhas de 500 elementos com desordem moderada (~19%–20%) caíssem em $O(n^2)$ e estourassem o teto de 12.000 operações no `ft_ps_tester`.
* **Limpeza de estilo e testes (`norminette`, `valgrind`, `ft_ps_tester`):** removemos comentários fora do 42 header e espaços em branco indevidos, corrigimos erros de indentação/assinatura/linhas longas e validamos memória (`0 B` de leak, `0` erros) e desempenho em 100 e 500 elementos.

## 1. Estado geral

| Etapa | Estado |
|---|---|
| Estruturas, Libft e Makefile | Implementados e compilando sem avisos (`-Wall -Wextra -Werror`) |
| Parsing de argumentos numéricos e tratamento de erros | Implementados e validados (`parse_args` refatorado para 4 argumentos) |
| Parsing de flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`) | Implementado (`parse_flags.c` + `t_config`) e validado no fluxo com `--bench` |
| 11 operações e contador `ops_stats` | Implementados e integrados ao `--bench`; revisão final de operações duplas pendente |
| Ordenação de 2 a 5 elementos | Implementada e validada (`sort_small.c`) |
| Simple — $O(n^2)$ | Implementado (`sort_simple.c`); válido para pilhas pequenas/baixa desordem, excede 12.000 ops em 500 elementos forçados |
| Indexação e `ft_sqrt()` | Implementados e validados (`indexing.c`, `sort_utils.c`) |
| Medium — $O(n\sqrt{n})$ (Chunk Sort) | **Implementado e validado** (`sort_medium.c`, `chunk_utils.c`) — 0 falhas em 100 e 500 elementos no `ft_ps_tester` |
| Complex — $O(n \log n)$ | Implementado e validado em 100 e 500 elementos no `ft_ps_tester` |
| Disorder (`compute_disorder`) e regime Adaptive | Implementados e ajustados (`0` falhas em 100 e 500 elementos no `ft_ps_tester`) |
| Saída do `--bench` (contagem das 11 operações no `stderr`) | Implementada em `parsing/bench.c`, chamada no `main.c` e aprovada no `ft_ps_tester` |
| Testes finais, Norminette e memória | Em andamento (memória zerada no `valgrind`; ajustes finais de `norminette` e limpeza em curso) |
| Checker próprio (bonus) | Pendente; opcional |

## 2. Estrutura atual de trabalho

```text
push_swap/
├── Makefile
├── push_swap.h
├── main.c
├── checker_linux           # binário de validação local (não entregar)
├── libft/
├── parsing/
│   ├── bench.c
│   ├── convert_numbers.c
│   ├── error_exit.c
│   ├── ft_split.c
│   ├── has_duplicate.c
│   ├── mini_libft.c
│   ├── ops_stats.c
│   ├── parse_args.c
│   └── parse_flags.c
├── stack/
│   ├── new_node_and_back.c
│   └── free_stack.c
├── operations/
│   ├── swap.c
│   ├── push.c
│   ├── rotate.c
│   └── reverse_rotate.c
└── sorting/
    ├── choose_adaptive.c
    ├── chunk_utils.c
    ├── compute_disorder.c
    ├── indexing.c
    ├── sort_complex.c
    ├── sort_complex_utils.c
    ├── sort_medium.c
    ├── sort_simple.c
    ├── sort_small.c
    └── sort_utils.c
```

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

### Chunks, operações e configuração de execução (`t_chunk`, `t_strategy`, `t_config`, `t_ops`)

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

typedef struct s_ops
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	total;
}	t_ops;
```

`t_config` armazena a estratégia escolhida (iniciada em `STRAT_ADAPTIVE`), se `--bench` está ativo (`0` ou `1`) e `arg_start`, o índice em `argv` onde terminam as flags e começam os números. Já `t_ops` centraliza os contadores das 11 operações e o `total`.

## 4. Funções implementadas

### Parsing, benchmark e gestão de memória

* `parse_flags.c` (`init_config`, `set_bench`, `set_strategy`, `parse_flags`): inicializa a configuração e processa as flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`), atualizando `arg_start`.
* `parse_args.c` (`parse_args`, `process_token`, `append_to_a`): recebe `(argc, argv, a, b)` já deslocados por `config.arg_start`, divide tokens por `' '`, valida e insere os nós no fim da stack A.
* `convert_numbers.c` / `has_duplicate.c`: validam formato numérico, limites `INT_MIN`/`INT_MAX` e números duplicados.
* `error_exit.c` / `free_stack()`: libertam memória e emitem `Error\n` no `stderr` em caso de falha.
* `ops_stats.c` e `bench.c`: registam as operações via `record_op()` / `get_ops_stats()` e imprimem o relatório completo em `fd 2` via `print_bench()` quando `cfg->bench` está ativo.

### Operações

| Operação | Efeito |
|---|---|
| `sa`, `sb`, `ss` | Trocar os dois primeiros elementos de A, B ou ambas |
| `pa`, `pb` | Transferir o topo de B para A ou de A para B |
| `ra`, `rb`, `rr` | Mover o primeiro elemento para o fundo |
| `rra`, `rrb`, `rrr` | Mover o último elemento para o topo |

**Revisão pendente:** confirmar na revisão final se `ss`, `rr` e `rrr` executam cada lado de forma independente quando apenas uma das stacks tem tamanho $\ge 2$.

### Ordenação pequena e utilitários (`sort_small.c`, `sort_utils.c`, `indexing.c`)

* `sort_two()`, `sort_three()`, `sort_four()`, `sort_five()`: ordenação dedicada para entradas de 2 a 5 elementos.
* `is_sorted()`: verifica ordem crescente sem modificar a stack.
* `find_min_pos()` / `push_min_to_b()`: localiza o menor valor e envia para B pelo caminho mais curto (`ra` ou `rra`).
* `ft_sqrt()`: calcula $\lfloor \sqrt{n} \rfloor$ sem risco de overflow.
* `assign_index()`: atribui a cada nó a sua posição ordenada (`0` a `n - 1`) sem alterar a ordem física da stack.

## 5. Simple — `sort_simple.c`

Adaptação de *Selection Sort* para duas stacks ($O(n^2)$ operações, $O(1)$ memória auxiliar):
1. Enquanto A tiver mais de 2 elementos, envia o menor para B (`push_min_to_b`).
2. Ordena os 2 restantes em A com `sort_two()`.
3. Devolve todos os elementos de B para A com `pa`.

## 6. Medium — `sort_medium.c` e `chunk_utils.c`

Estratégia baseada em *Chunk Sort* sobre os índices normalizados ($O(n\sqrt{n})$):

1. `assign_index(a)` calcula o índice final (`0` a `n - 1`) de cada nó.
2. `push_chunks_to_b()` define o tamanho dos chunks com base em `ft_sqrt(total)` e envia para B os elementos do intervalo atual (`push_chunk_node`), usando `rb(b)` nos elementos da metade inferior (`index < chunk.mid`) para pré-organizar B.
3. `push_back_to_a()` chama `push_max_to_a()` enquanto B não estiver vazia, trazendo o maior elemento ao topo via `rb` ou `rrb` antes de executar `pa(a, b)`.

## 7. Complex, Disorder e Adaptive

* **Complex (`sort_complex.c` e `sort_complex_utils.c` — $O(n \log n)$):** particionamento/ordenação sobre os índices normalizados com `partition_a`, `partition_b` e `get_pivot`.
* **Disorder (`compute_disorder`):** calcula a proporção de pares invertidos em A antes da ordenação, retornando `0.0` quando a pilha tem menos de 2 elementos.
* **Adaptive (`choose_adaptive` e `main.c`):** despacha a estratégia com base na desordem (com corte de `0.05` para `STRAT_SIMPLE` evitando $O(n^2)$ em 500 elementos com ~19%–20% de desordem, `0.5` para `STRAT_MEDIUM` e `STRAT_COMPLEX` acima disso).

## Instructions — Compilação e execução

```sh
make
./push_swap 42 -15 800 0 7
./push_swap "5 4 3 2 1"
./push_swap 1 2 3 4 5
./push_swap --simple 5 4 3 2 1
./push_swap --medium 42 -15 800 0 7
./push_swap --complex 42 -15 800 0 7
./push_swap --bench --adaptive 42 -15 800 0 7
```

* Entradas vazias, apenas com flags ou já ordenadas terminam com status `0` sem emitir operações.
* Flags desconhecidas ou argumentos inválidos/duplicados emitem `Error\n` no `stderr` e terminam com status `1`.

## 8. Testes já realizados

* **Corretude e estratégias (`ft_ps_tester`):** testes básicos (`PASS`), estratégia padrão alinhada ao adaptativo (`PASS`), relatório `--bench` no `stderr` (`PASS`) e `0` falhas em 100 elementos em todos os modos e em 500 elementos nos modos `MEDIUM`, `COMPLEX` e `ADAPTIVE` (em `SIMPLE` forçado com 500 elementos, as 100 falhas decorrem do limite de 12.000 operações para um algoritmo $O(n^2)$).
* **Memória (`valgrind`):** `0 B` de leaks, `0` erros de memória e sem crashes nos cenários testados.

## 9. Checklist até à entrega

* [x] Implementar e validar Medium (`sort_medium.c`, `chunk_utils.c`) com 100 e 500 números.
* [x] Implementar parsing de flags (`--simple`, `--medium`, `--complex`, `--adaptive`, `--bench`) em `parse_flags.c`.
* [x] Implementar Complex ($O(n \log n)$), `compute_disorder()`, modo Adaptive e relatório `--bench` no `stderr`.
* [x] Reduzir `parse_args` para 4 argumentos e validar memória com `valgrind`.
* [ ] Concluir passada final da `norminette` em todos os `.c` e `.h` sem nenhum erro.
* [ ] Rever operações duplas (`ss`, `rr`, `rrr`) e casos sem efeito.
* [ ] Limpar ficheiros temporários (`checker_linux`, `.o` / binários via `make fclean`) e testar `make re` sem avisos.
* [ ] Revisão final do repositório com a dupla e preparação para a defesa.

## 10. Contribuições da dupla

* **jeanbarr:** modularização do parsing (`parse_args.c`, `parse_flags.c`), estatísticas e saída do `--bench` (`ops_stats.c`, `bench.c`), integração no `main.c`, ajuste do `choose_adaptive.c`, desenvolvimento/validação do Medium (`chunk_utils.c`, `sort_medium.c`), `Makefile` e baterias de testes com `valgrind` e `ft_ps_tester`.
* **fercarva:** desenvolvimento das operações, ordenação de stacks pequenas (`sort_small.c`), utilitários (`sort_utils.c`), Simple (`sort_simple.c`), indexação (`indexing.c`) e estruturas iniciais do projeto.
* **Ambos:** revisão cruzada, integração, testes e preparação da defesa.

## Resources — Referências e utilização de IA

* Enunciado oficial *Push_swap* da 42.
* Manual de C e páginas locais `man 3 malloc`, `man 3 free`, `man 2 write`.
* Thomas H. Cormen et al., *Introduction to Algorithms* — análise de complexidade e algoritmos de ordenação.
* Assistentes de IA: utilizados para apoiar a refatoração de assinaturas/normas da 42, o desenho do `--bench`, o ajuste do limiar adaptativo e a análise dos resultados de testes.