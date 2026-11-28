*This project has been created as part of the 42 curriculum by fercarva, jeanbarr.*

# Push_swap — Diário de Bordo e Estado do Projeto

**Atualizado: 28/11/2026 | Fase atual: Complex de três partições integrado com optimizer V4-D; validação final em curso.**

## Description — Objetivo

Ordenar inteiros distintos em ordem crescente na stack A, utilizando uma stack auxiliar B e exclusivamente as 11 operações permitidas. O programa disponibiliza Simple, Medium, Complex e Adaptive, procurando reduzir o número de instruções emitidas, não apenas o tempo de execução em C.

## Diário de bordo

- **25/11:** integração do parsing de flags, cálculo de disorder e relatório `--bench`.
- **27/11:** integração do optimizer V4-D na base do Jean, refatoração e testes da versão anterior; união dos históricos Git preservando a árvore validada.
- **28/11:** substituição do Complex de duas metades por uma variante recursiva de Quick Sort com dois pivots, três grupos e quatro posições lógicas. Introdução de `sort_complex_move.c` e inclusão no Makefile. O optimizer continua a atuar sobre todas as estratégias.

## 1. Estado geral

| Componente | Estado |
|---|---|
| Parsing, stacks e operações | Implementados |
| Simple | Inserção pelo menor custo de movimentos |
| Medium | Chunks com janela dinâmica |
| Complex | Particionamento em três grupos |
| Adaptive | Limiares 20% e 50% |
| Optimizer V4-D | Integrado em todas as estratégias |
| Benchmark | Contagem das instruções finais em stderr |
| Compilação | Confirmada no ZIP de 28/11 |
| Norminette e Valgrind | Repetir na versão nova |
| Checker próprio | Não incluído no ZIP; bonus opcional |

## 2. Estrutura atual

```text
push_swap/
├── Makefile
├── README.md
├── push_swap.h
├── main.c
├── checker_linux (ferramenta local de validação)
├── stack/
│   ├── new_node_and_back.c
│   └── free_stack.c
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
├── operations/
│   ├── push.c
│   ├── swap.c
│   ├── rotate.c
│   └── reverse_rotate.c
├── sorting/
│   ├── choose_disorder.c
│   ├── chunk_utils.c
│   ├── sort_complex.c
│   ├── sort_complex_group.c
│   ├── sort_complex_move.c
│   ├── sort_complex_utils.c
│   ├── sort_medium.c
│   ├── sort_simple.c
│   ├── sort_small.c
│   └── sort_utils.c
└── optimize/
    ├── optimize_blocks.c
    ├── optimize_final.c
    ├── optimize_flush.c
    ├── optimize_merge.c
    ├── optimize_merge_ops.c
    ├── optimize_merge_utils.c
    ├── optimize_moves.c
    ├── optimize_normalize.c
    └── optimize_pairs.c
```

## 3. Estruturas de dados e fluxo

`t_node` guarda valor, índice ordenado e ligações anterior/seguinte. `t_stack` guarda head, tail, tamanho e referência ao buffer comum de movimentos. A lista duplamente ligada permite consultar ambas as extremidades. `t_config` guarda estratégia, flag bench e início dos argumentos numéricos. `t_program` reúne o estado da execução; `t_ops` contém os contadores; `t_moves` contém o vetor dinâmico de instruções, tamanho, capacidade e estado de erro.

O Complex acrescenta:

```c
typedef enum e_loc
{
    TOP_A, BOTTOM_A, TOP_B, BOTTOM_B
} t_loc;

typedef struct s_part
{
    t_loc loc;
    int size;
} t_part;

typedef struct s_split
{
    t_part min;
    t_part mid;
    t_part max;
} t_split;
```

Fluxo: parsing → disorder → seleção de estratégia → ordenação e registo de movimentos → optimizer → contagem final → impressão → libertação de memória. Entradas de 2 a 5 elementos usam rotinas dedicadas; entradas já ordenadas não geram operações.

## 4. Parsing e operações

`parse_flags` lê as flags iniciais (a última estratégia indicada prevalece); `parse_args` recebe argc/argv deslocados, separa tokens, valida inteiros e rejeita duplicados. `error_exit` trata erros e liberta recursos. As funções auxiliares de biblioteca estão em `parsing/`, não numa pasta libft separada.

| Operação | Efeito |
|---|---|
| sa / sb / ss | Trocar os dois primeiros elementos de A / B / ambas |
| pa / pb | Transferir o topo de B para A / de A para B |
| ra / rb / rr | Rodar A / B / ambas para cima |
| rra / rrb / rrr | Rodar A / B / ambas para baixo |

As operações alteram as stacks e registam instruções no buffer partilhado, em vez de imprimir imediatamente.

## 5. Simple — inserção pelo menor custo

1. Enviar elementos para B até restarem três em A e ordenar esses três.
2. Para cada candidato em B, localizar em A o menor valor superior ao candidato (ou o mínimo, quando não existe superior).
3. Avaliar quatro combinações de rotação direta/inversa de A e B. Para rotações no mesmo sentido, o custo é o máximo das magnitudes, pois `rr`/`rrr` combinam movimentos; em sentidos opostos, é a soma.
4. Executar o candidato mais barato e `pa`. A mantém uma ordem circular crescente.
5. Rodar A até o mínimo ficar no topo.

Justificação: no máximo n reinserções, cada uma com O(n) movimentos, dando limite O(n²) em instruções. A procura de candidatos percorre listas e pode custar O(n³) em tempo de CPU; não confundir estas métricas.

## 6. Medium — Chunk Sort com janela dinâmica

`assign_index` normaliza os valores para índices 0..n-1. A janela tem largura `floor(sqrt(n)) * 16 / 10` (mínimo 1). Com `pushed` como contador, índices até `pushed` são enviados para B e rodados; índices até `pushed + range` são enviados para o topo de B; os restantes rodam A. `rr` pode combinar rotações. Na recuperação, o maior índice restante é trazido pelo caminho mais curto; se o segundo maior estiver mais próximo, pode ser recuperado primeiro, seguido do maior e de `sa`.

A estratégia pretende obter O(n√n) movimentos através da janela proporcional à raiz quadrada e da organização de B. Esta é a justificação de desenho; os benchmarks não constituem uma demonstração formal de limite de pior caso para esta heurística.

## 7. Complex — Quick Sort de três partições

### 7.1. Motivação

O Complex anterior dividia cada grupo em duas metades e restaurava rotações para proteger os grupos exteriores. A nova abordagem usa topo e fundo de ambas as stacks como quatro posições lógicas. Não cria quatro stacks físicas. Cada grupo é descrito por localização e tamanho.

### 7.2. Motor de movimentos (`sort_complex_move.c`)

- `part_head`: devolve head ou tail conforme a origem.
- `part_min`: percorre apenas os elementos do grupo, usando next ou prev.
- `move_part`: traz o elemento do fundo para o topo se necessário, transfere entre stacks se necessário e roda para o fundo quando esse é o destino.
- `fix_part_loc`: trata um grupo que ocupa toda uma stack como grupo de topo, evitando rotações para aceder ao fundo.

Exemplo: BOTTOM_B → BOTTOM_A corresponde a `rrb`, `pa`, `ra`. Os custos nominais variam entre uma e três operações por transferência; operações sem efeito e optimizer podem reduzir esse custo.

### 7.3. Particionamento (`sort_complex_utils.c`)

| Origem | Destino MIN | Destino MID | Destino MAX | Pivots (%) |
|---|---|---|---|---|
| TOP_A | BOTTOM_B | TOP_B | BOTTOM_A | 18 / 58 |
| BOTTOM_A | BOTTOM_B | TOP_B | TOP_A | 22 / 66 |
| TOP_B | BOTTOM_B | BOTTOM_A | TOP_A | 42 / 62 |
| BOTTOM_B | TOP_B | BOTTOM_A | TOP_A | 46 / 62 |

Cada grupo contém índices consecutivos. Assim, os pivots são calculados a partir do menor índice mais a percentagem inteira do tamanho. Índices abaixo do primeiro pivot pertencem a MIN, entre os pivots a MID e a partir do segundo a MAX. `send_to` transfere um elemento e incrementa o tamanho do destino. `split_part` processa exatamente o tamanho do grupo original. Os destinos são distintos da origem, evitando misturar elementos ainda por processar.

Os pivots são heurísticos: favorecem destinos baratos, mas também influenciam os custos dos níveis seguintes. Por exemplo, 100 índices em TOP_A originam 18 menores, 40 intermédios e 42 maiores. O custo nominal inicial é 118 movimentos (18×2 + 40 + 42), antes da recursão e do optimizer.

### 7.4. Recursão (`sort_complex.c`)

`sort_part` ignora grupos vazios, corrige a localização, evita trabalhar num grupo já ordenado em TOP_A e resolve diretamente grupos até três elementos. Caso contrário, divide e chama recursivamente MAX, MID, MIN, por esta ordem. Cada grupo termina ordenado no topo de A, pelo que os menores ficam acima dos maiores. As rotinas de três elementos preservam os elementos exteriores ao grupo.

### 7.5. Complexidade

Cada partição processa n elementos com custo constante de movimentos por elemento. Os cortes fixos mantêm os subgrupos proporcionalmente menores (para grupos pequenos há arredondamento e casos base). O trabalho total por nível é O(n), a profundidade O(log n), e o limite de movimentos é O(n log n). Não é necessário que os três grupos sejam iguais. A base do logaritmo não altera a classe assintótica. A indexação inicial e o cálculo de disorder percorrem pares em O(n²) tempo de CPU, sem emitir instruções; o optimizer também tem custo computacional próprio.

## 8. Adaptive e Benchmark

Disorder = pares invertidos / (n(n−1)/2), com zero para n < 2. É calculado antes de ordenar.

| Disorder | Estratégia |
|---|---|
| d < 0,20 | Simple |
| 0,20 ≤ d < 0,50 | Medium |
| d ≥ 0,50 | Complex |

Adaptive é o modo padrão. `--bench` escreve em stderr a percentagem de disorder, estratégia/classe, total e contagem das 11 instruções finais. O stdout contém apenas instruções. A estratégia indicada no relatório é a selecionada, mesmo quando a rotina dedicada a entradas pequenas é usada.

## 9. Optimizer V4-D

O optimizer trabalha sobre o vetor de instruções depois de ordenar as stacks em memória. Não escolhe a estratégia nem altera os pivots.

| Ficheiro | Responsabilidade |
|---|---|
| optimize_moves.c | Gestão do buffer dinâmico e impressão |
| optimize_pairs.c | Cancelar ou combinar pares adjacentes |
| optimize_blocks.c | Agrupar rotações e acompanhar tamanhos |
| optimize_normalize.c | Reduzir rotações módulo tamanho e comparar custos |
| optimize_flush.c | Emitir rotações combinadas e restantes |
| optimize_merge.c | Separar blocos delimitados por pushes |
| optimize_merge_ops.c | Separar e recombinar instruções de A e B |
| optimize_merge_utils.c | Otimizar sequências locais e aceitar apenas blocos menores |
| optimize_final.c | Repetir as etapas até o tamanho estabilizar |

Exemplos: `ra rra` cancela; `sa sb` torna-se `ss`; `ra rb` torna-se `rr`. Uma rotação completa não altera a stack. Em blocos sem pushes, operações de stacks distintas são independentes; podemos preservar a ordem interna de cada stack e combinar operações compatíveis. Os pushes delimitam blocos porque alteram a distribuição dos elementos. O merge é guloso, não uma procura exaustiva da sequência ótima. A redução do número de instruções preserva o limite superior de movimentos da estratégia original; não implica que o tempo de CPU do optimizer seja linear.

## Instructions — Compilação e execução

```bash
make
./push_swap 5 4 3 2 1
./push_swap --simple 42 -15 800 0 7
./push_swap --medium 42 -15 800 0 7
./push_swap --complex 42 -15 800 0 7
./push_swap --adaptive --bench 42 -15 800 0 7
./push_swap "5 3 1" "4 2"
chmod +x checker_linux
ARG="5 4 3 2 1"
./push_swap $ARG | ./checker_linux $ARG
make clean
make fclean
make re
```

O Makefile usa `cc -Wall -Wextra -Werror`. Sem argumentos ou apenas com flags, termina sem operações. Argumentos inválidos, duplicados e valores fora dos limites de int são rejeitados com `Error` em stderr.

## 10. Testes realizados — versão de 28/11

Compilação bem-sucedida do ZIP `a(1).zip` e teste Complex de 10 elementos: 25 operações, checker OK. Bateria independente com Python, seed 42, 100 permutações aleatórias por combinação de tamanho e estratégia (amostras distintas entre estratégias). Limite de 5 segundos por processo. Resultados medidos sobre as instruções finais:

| N | Estratégia | Checker OK | Média | Mín. | Máx. | Excelentes |
|---:|---|---:|---:|---:|---:|---:|
| 100 | Simple | 100/100 | 574,73 | 498 | 642 | 100/100 |
| 100 | Medium | 100/100 | 538,36 | 507 | 580 | 100/100 |
| 100 | Complex | 100/100 | 550,34 | 531 | 573 | 100/100 |
| 100 | Adaptive | 100/100 | 542,41 | 496 | 567 | 100/100 |
| 500 | Simple | 100/100 | 5161,83 | 4924 | 5500 | 99/100 |
| 500 | Medium | 100/100 | 4574,54 | 4374 | 4760 | 100/100 |
| 500 | Complex | 100/100 | 3857,20 | 3797 | 3939 | 100/100 |
| 500 | Adaptive | 100/100 | 4236,97 | 3802 | 4709 | 100/100 |

Total: 800/800 checker OK; 799/800 abaixo dos limiares estritos de excelência (<700 para 100 e <5500 para 500). O Simple com 5500 não cumpre o limiar estrito, mas ordenou corretamente. Os resultados não garantem desempenho para todas as permutações.

O script de 10.000 testes com 100 números foi fornecido, mas o respetivo output final não foi disponibilizado; não atribuímos resultados não confirmados. Para repetir a bateria, usar `shuf -i 1-100 -n 100`, guardar stdout num ficheiro temporário, verificar com checker e contar linhas. Usar `--complex` para isolar o novo algoritmo, ou omitir a flag para testar Adaptive.

## 11. Checklist até à entrega

- [x] Implementar as quatro estratégias e optimizer.
- [x] Integrar o Complex de três partições e compilar.
- [x] Validar 800 casos aleatórios na versão nova.
- [ ] Registar output dos 10.000 testes.
- [ ] Remover comentários de desenvolvimento fora dos headers e executar Norminette na versão nova.
- [ ] Repetir Valgrind, casos inválidos, extremos e fronteiras Adaptive.
- [ ] Verificar falhas de alocação, clone limpo e ausência de relink.
- [ ] Confirmar publicação Git da versão nova e excluir artefactos locais da entrega.
- [ ] Rever documentação e preparar defesa em dupla.

## 12. Contribuições da dupla

- **jeanbarr:** parsing, flags, benchmark, integração do main, Medium, evolução do Simple, Makefile e testes, conforme o diário anterior e os ficheiros atuais.
- **fercarva:** estruturas iniciais, operações, rotinas pequenas, utilitários, Complex inicial, desenvolvimento e integração do optimizer V4-D e evolução do Complex com assistência de IA.
- **Ambos:** revisão cruzada, integração e validação. Confirmar a atribuição final em dupla antes da entrega.

## Resources — Referências e utilização de IA

- Enunciado oficial Push_swap da 42, versão 1.2.
- Manuais locais de C: `man 3 malloc`, `man 3 free`, `man 2 write`.
- Thomas H. Cormen et al., *Introduction to Algorithms*: divisão e conquista, ordenação e análise de complexidade.
- Assistentes de IA: apoio à discussão de algoritmos, parsing, refatoração, optimizer, scripts de teste, documentação e implementação do novo Complex com quatro posições e pivots heurísticos. A dupla deve compreender, verificar e conseguir defender o código produzido com esse apoio.
