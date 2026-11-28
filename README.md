*This project has been created as part of the 42 curriculum by fercarva, jeanbarr.*

<h1 align="center">🔀 push_swap</h1>

## 📖 Description
<table> <tr> <td valign="middle" width="50%">
push_swap sorts a stack of distinct integers using a second stack and a very small set of instructions, and prints the <strong>shortest list of instructions it can find</strong>. The goal of the project is to learn about <strong>algorithm complexity</strong>: the same input can be sorted with very different numbers of operations, and the choice of algorithm decides how many.

</td> <td align="center" valign="middle" width="50%"> <img src="https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExYjNuNGNwZmZyYTlnZzhwanNkOHh5Mnk2bTNleWJsOGNqN3QxZXhhOCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/iEF5YoD7B3B6mRjKir/giphy.gif" alt="push_swap animation" width="100%"> </td> </tr> </table>

The program takes the integers as arguments, sorts stack **A** using stack **B**, and writes one instruction per line to the standard output. Errors are reported with `Error` on the standard error.

| Operation | Effect |
|---|---|
| `sa` / `sb` / `ss` | Swap the first two elements of A / B / both |
| `pa` / `pb` | Push the top of B onto A / the top of A onto B |
| `ra` / `rb` / `rr` | Rotate A / B / both up (the first element becomes the last) |
| `rra` / `rrb` / `rrr` | Rotate A / B / both down (the last element becomes the first) |

The program has four sorting strategies, chosen by a flag or automatically:

| Strategy | Flag | Moves (class) | Idea |
|---|---|---|---|
| Simple | `--simple` | O(n²) | Insert each element at the cheapest position |
| Medium | `--medium` | O(n√n) | Chunks with a sliding window |
| Complex | `--complex` | O(n log n) | Quick Sort with three partitions |
| Adaptive | `--adaptive` (default) | depends | Picks one of the above from the *disorder* of the input |

Whatever the strategy, a final **optimizer** shortens the list of instructions before it is printed.

---

## ⚙️ Instructions

**Requirements:** `cc` and `make`.

| Command | Description |
|---|---|
| `make` | Builds `push_swap` |
| `make clean` | Removes the object files |
| `make fclean` | Removes the object files and `push_swap` |
| `make re` | Rebuilds everything from scratch |

Everything is compiled with `-Wall -Wextra -Werror`.

```sh
make
./push_swap 5 4 3 2 1
./push_swap "5 3 1" "4 2"
./push_swap --simple 42 -15 800 0 7
./push_swap --medium 42 -15 800 0 7
./push_swap --complex 42 -15 800 0 7
./push_swap --adaptive --bench 42 -15 800 0 7
```

Flags must come **before** the numbers. If several strategy flags are given, the last one wins. With `--bench`, a report is written to the standard **error**, so the standard output still contains only instructions.

To check a result with the provided checker:

```sh
chmod +x checker_linux
ARG="5 4 3 2 1"
./push_swap $ARG | ./checker_linux $ARG     # prints OK or KO
./push_swap $ARG | wc -l                    # number of instructions
```

Invalid arguments (letters, duplicates, values outside the `int` range) print `Error` and return a non-zero exit code. With no arguments, or only flags, the program prints nothing.

---

## 🔧 Program description

### Files

| Folder / file | Role |
|---|---|
| `main.c` | Reads the flags and arguments, runs the sort, calls the optimizer, prints |
| `push_swap.h` | Structures, prototypes and the strategy enum |
| `stack/` | Creating nodes, adding them to the stack, freeing |
| `parsing/` | Flags, argument parsing, validation, duplicates, errors, `--bench` and operation counters, small libft helpers |
| `operations/` | The 11 instructions (`push`, `swap`, `rotate`, `reverse_rotate`) |
| `sorting/` | Disorder and strategy choice, small sorts (2 to 5), Simple, Medium, Complex |
| `optimize/` | The optimizer, split into small steps |

The libft helpers needed (`ft_split`, `ft_strlen`...) are inside `parsing/`, not in a separate libft folder. The only system functions used are `write`, `malloc` and `free`.

### Data structures and flow

A stack is a **doubly linked list** (`t_stack`: head, tail, size, and a pointer to the shared list of moves). Each node (`t_node`) keeps its value, its **index** in the sorted order, and the previous/next links. Having both ends of the list makes the reverse rotations and the "closest to the bottom" checks cheap.

The operations do not print anything. They change the stacks and **record** the instruction in a shared dynamic array (`t_moves`). The flow is:

```
parse flags → parse numbers → compute disorder → choose strategy
   → sort (record moves) → optimize → count → print → free
```

*Why this choice:* recording first and printing later is what makes the optimizer possible, because it needs to see the whole list of instructions at once.

Inputs of 2 to 5 numbers use dedicated routines, and an already sorted input produces no instructions.

### Simple: insertion at the cheapest cost

1. Push elements to B until three remain in A, and sort those three.
2. For each element in B, find in A the smallest value above it (or the minimum if there is none).
3. Compute four combinations of rotating A and B (up or down). If both rotate in the same direction, the cost is the larger of the two (`rr` / `rrr` do both at once); if they go in opposite directions, the cost is the sum.
4. Run the cheapest candidate, then `pa`.
5. Rotate A until the minimum is on top.

*Why this choice:* it is simple to reason about, and the cost of each move is explicit. At most n elements are inserted, each with O(n) moves, so the limit is **O(n²) instructions**. Finding the cheapest candidate walks the lists and can cost O(n³) CPU time; these are different metrics.

### Medium: chunk sort with a dynamic window

Values are first replaced by their index (0 to n-1). The window has width `floor(sqrt(n)) * 16 / 10` (at least 1). With `pushed` as the number of elements already sent to B:

- an index up to `pushed` goes to B and is rotated down (to the bottom of B);
- an index up to `pushed + width` goes to B and stays on top;
- any other element makes A rotate.

`rr` is used when both stacks need to rotate. To bring everything back, the largest remaining index is moved to the top of B by the shortest path and pushed to A; if the second largest is closer, it can be recovered first, followed by the largest and a `sa`.

*Why this choice:* a window proportional to √n keeps B partially organized, which aims at O(n√n) moves. This is a design argument, not a formal worst-case proof; the benchmarks below are measurements.

### Complex: Quick Sort with three partitions

The older version split every group into two halves and had to undo rotations to protect the outer groups. The current version uses the **top and the bottom of both stacks** as four logical positions. It does not create four physical stacks: each group is only a location and a size.

```c
typedef enum e_loc { TOP_A, BOTTOM_A, TOP_B, BOTTOM_B } t_loc;
typedef struct s_part  { t_loc loc; int size; } t_part;
typedef struct s_split { t_part min; t_part mid; t_part max; } t_split;
```

**Moving elements** (`sort_complex_move.c`): `move_part` brings an element from the bottom to the top if needed, transfers it between stacks if needed, and rotates it to the bottom when that is the destination. For example, BOTTOM_B to BOTTOM_A is `rrb`, `pa`, `ra`. Each transfer costs between one and three operations before optimization.

**Partitioning** (`sort_complex_utils.c`): a group is split by two pivots, computed from its smallest index plus a percentage of its size. The percentages and destinations depend on where the group is:

| Origin | MIN goes to | MID goes to | MAX goes to | Pivots (%) |
|---|---|---|---|---|
| TOP_A | BOTTOM_B | TOP_B | BOTTOM_A | 18 / 58 |
| BOTTOM_A | BOTTOM_B | TOP_B | TOP_A | 22 / 66 |
| TOP_B | BOTTOM_B | BOTTOM_A | TOP_A | 42 / 62 |
| BOTTOM_B | TOP_B | BOTTOM_A | TOP_A | 46 / 62 |

The destinations are always different from the origin, so already processed elements are never mixed with the ones still to process. For 100 elements on TOP_A this gives 18 small, 40 medium and 42 large elements.

**Recursion** (`sort_complex.c`): `sort_part` ignores empty groups, fixes the location of a group that fills a whole stack, skips a group that is already sorted on top of A, and solves groups of up to three elements directly. Otherwise it splits and recurses on MAX, then MID, then MIN, so each group ends sorted on top of A.

*Why this choice:* each level of recursion handles n elements with a constant number of moves each, and the groups get proportionally smaller, so the depth is O(log n) and the total is **O(n log n) moves**. The pivots are heuristic: they were tuned to favour cheap destinations, not derived from a proof.

### Adaptive and benchmark

**Disorder** = inverted pairs / (n(n−1)/2), and 0 when n < 2. It is computed before sorting.

| Disorder | Strategy |
|---|---|
| below 0.20 | Simple |
| 0.20 to below 0.50 | Medium |
| 0.50 or more | Complex |

*Why this choice:* the subject fixes the two thresholds (0.2 and 0.5) and the complexity class allowed in each regime, so our design choice is which technique runs in each one. Low disorder uses Simple (O(n²)), medium disorder uses Medium (O(n√n)) and high disorder uses Complex (O(n log n)). The upper bound of Adaptive is therefore the bound of the regime selected, and the disorder is measured before any move is made.

`--bench` writes to the standard error: the disorder, the strategy and its class, the total, and the count of each of the 11 instructions. The strategy shown is the one selected, even when the small-input routine is used.

### Optimizer

The optimizer works on the recorded list of instructions, after the stacks are already sorted in memory. It does not choose the strategy or change the pivots.

| Step | What it does |
|---|---|
| Pairs | Cancels (`ra rra`) or merges (`sa sb` to `ss`, `ra rb` to `rr`) adjacent instructions |
| Blocks | Groups rotations and keeps track of the stack sizes |
| Normalize | Reduces rotations modulo the stack size and keeps the cheaper direction |
| Merge | Splits the list at every push, then interleaves independent instructions of A and B so they can become `rr` / `rrr` / `ss` |
| Final | Repeats everything until the length stops decreasing |

*Why this choice:* between two pushes, instructions on different stacks do not depend on each other, so they can be reordered and combined safely. The merge is greedy, not an exhaustive search for the best sequence. Fewer instructions keeps the move limit of the original strategy, but it does not mean the optimizer itself runs in linear time.

### Complexity summary (time and space)

All time bounds count **Push_swap instructions**, as the subject requires, not CPU time.

| Strategy | Instructions (upper bound) | Extra space |
|---|---|---|
| Simple | O(n²) | O(1) besides the stacks and the move buffer |
| Medium | O(n√n) | O(1) besides the stacks and the move buffer |
| Complex | O(n log n) | O(log n) recursion depth, plus the same structures |
| Adaptive | the bound of the selected regime: O(n²), O(n√n) or O(n log n) | same as the selected strategy |

Space, common to all strategies: the two stacks hold at most n nodes in total, so O(n). The recorded instructions are kept until the end in one dynamic array, so its size is O(m), where m is the number of instructions (up to O(n²) for Simple). The optimizer reorganizes that same array and does not allocate other buffers. Reading the arguments uses temporary strings that are freed before sorting.

CPU time is a separate metric: the disorder and the value indexing each compare all pairs, so they cost O(n²) CPU time but emit no instructions.

### Limitations

- The bonus `checker` program is not implemented; the provided `checker_linux` is used for validation.
- The result is not guaranteed to be the shortest possible for every permutation.

---

## 🧪 Testing

### Results by strategy (seed 42)

An independent Python script ran 100 random permutations per size and strategy, with a 5 second limit per process. Counts are the final instructions.

| N | Strategy | Checker OK | Average | Min | Max | Excellent |
|---:|---|---:|---:|---:|---:|---:|
| 100 | Simple | 100/100 | 574.73 | 498 | 642 | 100/100 |
| 100 | Medium | 100/100 | 538.36 | 507 | 580 | 100/100 |
| 100 | Complex | 100/100 | 550.34 | 531 | 573 | 100/100 |
| 100 | Adaptive | 100/100 | 542.41 | 496 | 567 | 100/100 |
| 500 | Simple | 100/100 | 5161.83 | 4924 | 5500 | 99/100 |
| 500 | Medium | 100/100 | 4574.54 | 4374 | 4760 | 100/100 |
| 500 | Complex | 100/100 | 3857.20 | 3797 | 3939 | 100/100 |
| 500 | Adaptive | 100/100 | 4236.97 | 3802 | 4709 | 100/100 |

800/800 passed the checker. "Excellent" means fewer than 700 instructions for 100 numbers and fewer than 5500 for 500 numbers; 799/800 met it (Simple with exactly 5500 sorted correctly but missed the strict limit).

### Final validation

A shell script (`shuf` + `checker_linux`) ran 41 checks, all passed:

- **Invalid input:** duplicates, letters, `+`, `-`, `--`, values outside `int`, empty string. All print `Error` on stderr, nothing on stdout, and exit with a non-zero code.
- **Edge cases:** `INT_MIN`, `INT_MAX`, a single number and sorted inputs produce 0 instructions.
- **Flags:** the four strategies, `--bench` (stdout unchanged), flags with no numbers.
- **Adaptive boundaries:** disorder of 0%, 2%, 5.8%, 55% and 100% selected the expected strategy.
- **Sizes 1 to 20:** all strategies passed the checker.
- **100 numbers, 200 runs (Adaptive):** 200/200 OK, average 544, maximum 575.
- **500 numbers, 200 runs (Adaptive):** 200/200 OK, average 4242, maximum 4785.
- **Makefile:** no relink, `fclean` and `re` work.
- **Norminette:** all `.c` files and the header pass.
- **Memory:** AddressSanitizer and UBSan reported no leaks, overflows or undefined behaviour in all strategies and invalid cases. `valgrind` was also used by the team during development.

**10,000 runs with 100 numbers (Adaptive):** 10,000/10,000 checker OK, average 542 instructions, maximum 583 (limit: fewer than 700), no run slower than 2 seconds.

These are measurements on random samples; they do not guarantee the same performance for every permutation.

---

## 👥 Contributions

### fercarva

- **Foundations:** the initial project structures (stacks and nodes), the 11 operations, and small helper routines and utilities (`sort_utils.c`).
- **Small and simple sorts:** sorting of small stacks (`sort_small.c`), the first version of Simple (`sort_simple.c`) and the indexing of values.
- **Complex:** the first version of Complex, and its later evolution into the three-partition Quick Sort with four logical positions (`sort_complex*.c`), with AI assistance.
- **Optimizer:** development and integration of the V4-D optimizer (`optimize/`), which now runs on every strategy.

### jeanbarr

- **Parsing and flags:** modular argument parsing and validation (`parse_args.c`, `parse_flags.c`), including the strategy and `--bench` flags.
- **Benchmark:** operation counters and the `--bench` report written to the standard error (`ops_stats.c`, `bench.c`).
- **Integration:** wiring everything together in `main.c`, and the Adaptive strategy selection (`choose_adaptive`).
- **Sorting:** development and validation of Medium (`chunk_utils.c`, `sort_medium.c`) and later improvements to Simple.
- **Build and tests:** the `Makefile` and the test batteries, using `valgrind`, `ft_ps_tester` and `ft_ps_visu`.

### Both

- Cross review of each other's code, integration of the parts, validation of the final version, and preparation of the defence as a pair.

---

## 📚 Resources

- push_swap subject provided by the school (42).
- C manual pages: `man 3 malloc`, `man 3 free` and `man 2 write`.
- Thomas H. Cormen et al., *Introduction to Algorithms*: complexity analysis and sorting algorithms.
- Aditya Y. Bhargava, *Grokking Algorithms: An Illustrated Guide for Programmers and Other Curious People*.
- Community tools used for testing and visualization: `ft_ps_tester` and `ft_ps_visu`.

### How AI was used

AI was used as a learning and review tool during this project, for these tasks:

- **Refactoring:** adapting function signatures and code to the 42 Norm.
- **`--bench` design:** discussing what the report should show and how to count each instruction.
- **Adaptive selection:** discussing how the measured disorder maps to a strategy, and analysing the test results.
- **Complex and optimizer:** discussing the three-partition algorithm and the optimizer steps.
- **Tests and documentation:** writing the final validation script and this README.

All the code and documentation were reviewed by the team, who understand it and can defend it.