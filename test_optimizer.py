
import random
import subprocess
import sys

# ==========================================
# CONFIGURACAO
# ==========================================

SIZE = int(sys.argv[1]) if len(sys.argv) > 1 else 100
TESTS = int(sys.argv[2]) if len(sys.argv) > 2 else 1000

random.seed(42)

# ==========================================
# REGRAS V1
# ==========================================

combine_v1 = {
    ("ra", "rb"): "rr",
    ("rb", "ra"): "rr",
    ("rra", "rrb"): "rrr",
    ("rrb", "rra"): "rrr",
    ("sa", "sb"): "ss",
    ("sb", "sa"): "ss",
}

cancel = {
    ("ra", "rra"),
    ("rra", "ra"),
    ("rb", "rrb"),
    ("rrb", "rb"),
    ("rr", "rrr"),
    ("rrr", "rr"),
    ("sa", "sa"),
    ("sb", "sb"),
    ("ss", "ss"),
}

# ==========================================
# REGRAS V2
# ==========================================

combine_v2 = combine_v1.copy()

combine_v2.update({
    ("rr", "rra"): "rb",
    ("rr", "rrb"): "ra",
    ("rrr", "ra"): "rrb",
    ("rrr", "rb"): "rra",
    ("rra", "rr"): "rb",
    ("rrb", "rr"): "ra",
    ("ra", "rrr"): "rrb",
    ("rb", "rrr"): "rra",
    ("ss", "sa"): "sb",
    ("ss", "sb"): "sa",
    ("sa", "ss"): "sb",
    ("sb", "ss"): "sa",
})

# ==========================================
# OTIMIZACAO DE PARES - V1 E V2
# ==========================================

def optimize_pairs(operations, combine):
    result = []

    for op in operations:
        result.append(op)

        while len(result) >= 2:
            pair = (result[-2], result[-1])

            if pair in cancel:
                result.pop()
                result.pop()

            elif pair in combine:
                result.pop()
                result.pop()
                result.append(combine[pair])

            else:
                break

    return result


# ==========================================
# OTIMIZACAO DE BLOCOS - V3
# ==========================================

rotations = {
    "ra":  (1, 0),
    "rb":  (0, 1),
    "rr":  (1, 1),
    "rra": (-1, 0),
    "rrb": (0, -1),
    "rrr": (-1, -1),
}


def flush_rotations(a, b):
    result = []

    if a > 0 and b > 0:
        common = min(a, b)
        result.extend(["rr"] * common)
        a -= common
        b -= common

    elif a < 0 and b < 0:
        common = min(-a, -b)
        result.extend(["rrr"] * common)
        a += common
        b += common

    if a > 0:
        result.extend(["ra"] * a)

    elif a < 0:
        result.extend(["rra"] * (-a))

    if b > 0:
        result.extend(["rb"] * b)

    elif b < 0:
        result.extend(["rrb"] * (-b))

    return result


def optimize_blocks(operations):
    result = []
    a = 0
    b = 0

    for op in operations:
        if op in rotations:
            da, db = rotations[op]
            a += da
            b += db

        else:
            result.extend(flush_rotations(a, b))
            a = 0
            b = 0
            result.append(op)

    result.extend(flush_rotations(a, b))

    return result


def optimize_v3(operations):
    result = operations.copy()

    while True:
        before = len(result)

        result = optimize_pairs(result, combine_v2)
        result = optimize_blocks(result)
        result = optimize_pairs(result, combine_v2)

        if len(result) == before:
            return result


# ==========================================
# OPERACOES INDEPENDENTES
# ==========================================

split_ops = {
    "sa": ("sa", None),
    "sb": (None, "sb"),
    "ss": ("sa", "sb"),
    "ra": ("ra", None),
    "rb": (None, "rb"),
    "rr": ("ra", "rb"),
    "rra": ("rra", None),
    "rrb": (None, "rrb"),
    "rrr": ("rra", "rrb"),
}

merge_ops = {
    ("sa", "sb"): "ss",
    ("ra", "rb"): "rr",
    ("rra", "rrb"): "rrr",
}


# ==========================================
# NORMALIZAR OPERACOES DE UMA STACK
# ==========================================

def normalize_stack(
    operations, size, forward, reverse, swap_op
):
    result = []
    rotation = 0

    if size < 2:
        return result

    def flush():
        nonlocal rotation

        rotation %= size

        if rotation <= size - rotation:
            result.extend([forward] * rotation)

        else:
            result.extend([reverse] * (size - rotation))

        rotation = 0

    for op in operations:
        if op == forward:
            rotation += 1

        elif op == reverse:
            rotation -= 1

        elif op == swap_op:
            flush()

            if result and result[-1] == swap_op:
                result.pop()

            else:
                result.append(op)

    flush()

    return result


# ==========================================
# COMBINACAO COM PROGRAMACAO DINAMICA
# ==========================================

def merge_independent(a_ops, b_ops):
    na = len(a_ops)
    nb = len(b_ops)

    dp = [[0] * (nb + 1) for _ in range(na + 1)]

    for i in range(na - 1, -1, -1):
        for j in range(nb - 1, -1, -1):
            pair = (a_ops[i], b_ops[j])

            if pair in merge_ops:
                dp[i][j] = 1 + dp[i + 1][j + 1]

            else:
                dp[i][j] = max(
                    dp[i + 1][j],
                    dp[i][j + 1]
                )

    result = []
    i = 0
    j = 0

    while i < na and j < nb:
        pair = (a_ops[i], b_ops[j])

        if pair in merge_ops:
            result.append(merge_ops[pair])
            i += 1
            j += 1

        elif dp[i + 1][j] >= dp[i][j + 1]:
            result.append(a_ops[i])
            i += 1

        else:
            result.append(b_ops[j])
            j += 1

    result.extend(a_ops[i:])
    result.extend(b_ops[j:])

    return result


# ==========================================
# COMBINACAO GREEDY
# ==========================================

def merge_greedy(a_ops, b_ops):
    result = []
    i = 0
    j = 0

    while i < len(a_ops) and j < len(b_ops):
        pair = (a_ops[i], b_ops[j])

        if pair in merge_ops:
            result.append(merge_ops[pair])
            i += 1
            j += 1

        elif (
            i + 1 < len(a_ops)
            and (a_ops[i + 1], b_ops[j]) in merge_ops
        ):
            result.append(a_ops[i])
            i += 1

        elif (
            j + 1 < len(b_ops)
            and (a_ops[i], b_ops[j + 1]) in merge_ops
        ):
            result.append(b_ops[j])
            j += 1

        else:
            result.append(a_ops[i])
            i += 1

    result.extend(a_ops[i:])
    result.extend(b_ops[j:])

    return result


# ==========================================
# OTIMIZACAO INDEPENDENTE CONFIGURAVEL
# ==========================================

def independent_variant(
    operations, size, normalize, merge_function
):
    result = []
    a_ops = []
    b_ops = []

    size_a = size
    size_b = 0

    def flush():
        if normalize:
            a = normalize_stack(
                a_ops, size_a, "ra", "rra", "sa"
            )

            b = normalize_stack(
                b_ops, size_b, "rb", "rrb", "sb"
            )

        else:
            a = a_ops
            b = b_ops

        result.extend(merge_function(a, b))

        a_ops.clear()
        b_ops.clear()

    for op in operations:
        if op in ("pa", "pb"):
            flush()
            result.append(op)

            if op == "pa" and size_b > 0:
                size_a += 1
                size_b -= 1

            elif op == "pb" and size_a > 0:
                size_a -= 1
                size_b += 1

        else:
            op_a, op_b = split_ops[op]

            if op_a is not None:
                a_ops.append(op_a)

            if op_b is not None:
                b_ops.append(op_b)

    flush()

    return result


def optimize_independent(operations, size):
    return independent_variant(
        operations, size, True, merge_independent
    )


# ==========================================
# CANCELAR PUSHES INVERSOS
# ==========================================

def cancel_pushes(operations, size):
    result = []
    effective = []

    size_a = size
    size_b = 0

    for op in operations:
        executed = False

        if op == "pa" and size_b > 0:
            size_a += 1
            size_b -= 1
            executed = True

        elif op == "pb" and size_a > 0:
            size_a -= 1
            size_b += 1
            executed = True

        if (
            result
            and executed
            and effective[-1]
            and (result[-1], op) in (
                ("pa", "pb"),
                ("pb", "pa")
            )
        ):
            result.pop()
            effective.pop()

        else:
            result.append(op)
            effective.append(executed)

    return result


# ==========================================
# NORMALIZAR APENAS BLOCOS DE ROTACOES
# ==========================================

def normalize_only(operations, size):
    result = []
    a = 0
    b = 0

    size_a = size
    size_b = 0

    def shortest(rotation, stack_size):
        if stack_size < 2:
            return 0

        rotation %= stack_size

        if rotation > stack_size - rotation:
            rotation -= stack_size

        return rotation

    def flush():
        nonlocal a, b

        result.extend(
            flush_rotations(
                shortest(a, size_a),
                shortest(b, size_b)
            )
        )

        a = 0
        b = 0

    for op in operations:
        if op in rotations:
            da, db = rotations[op]
            a += da
            b += db

        else:
            flush()
            result.append(op)

            if op == "pa" and size_b > 0:
                size_a += 1
                size_b -= 1

            elif op == "pb" and size_a > 0:
                size_a -= 1
                size_b += 1

    flush()

    return result


# ==========================================
# V4-A - APENAS CANCELAMENTO DE PUSHES
# ==========================================

def optimize_v4_a(operations, size):
    result = optimize_v3(operations)

    while True:
        candidate = cancel_pushes(result, size)
        candidate = optimize_v3(candidate)

        if len(candidate) >= len(result):
            return result

        result = candidate


# ==========================================
# V4-B - APENAS NORMALIZACAO DE ROTACOES
# ==========================================

def optimize_v4_b(operations, size):
    result = optimize_v3(operations)

    while True:
        candidate = normalize_only(result, size)
        candidate = optimize_v3(candidate)

        if len(candidate) >= len(result):
            return result

        result = candidate


# ==========================================
# V4-C - OPERACOES INDEPENDENTES COM DP
# ==========================================

def optimize_v4_c(operations, size):
    result = optimize_v3(operations)

    while True:
        candidate = independent_variant(
            result, size, False, merge_independent
        )

        candidate = optimize_v3(candidate)

        if len(candidate) >= len(result):
            return result

        result = candidate


# ==========================================
# V4-D - TODAS AS TECNICAS COM GREEDY
# ==========================================

def optimize_v4_d(operations, size):
    result = optimize_v3(operations)

    while True:
        candidate = independent_variant(
            result, size, True, merge_greedy
        )

        candidate = cancel_pushes(candidate, size)
        candidate = optimize_v3(candidate)

        if len(candidate) >= len(result):
            return result

        result = candidate


# ==========================================
# V4 COMPLETA - PROGRAMACAO DINAMICA
# ==========================================

def optimize_v4(operations, size):
    result = optimize_v3(operations)

    while True:
        candidate = optimize_independent(result, size)
        candidate = cancel_pushes(candidate, size)
        candidate = optimize_v3(candidate)

        if len(candidate) >= len(result):
            return result

        result = candidate


# ==========================================
# CHECKER INDEPENDENTE
# ==========================================

def swap(stack):
    if len(stack) >= 2:
        stack[0], stack[1] = stack[1], stack[0]


def rotate(stack):
    if len(stack) >= 2:
        stack.append(stack.pop(0))


def reverse_rotate(stack):
    if len(stack) >= 2:
        stack.insert(0, stack.pop())


def push(dst, src):
    if src:
        dst.insert(0, src.pop(0))


def checker(numbers, operations):
    a = numbers.copy()
    b = []

    for op in operations:
        if op == "sa":
            swap(a)

        elif op == "sb":
            swap(b)

        elif op == "ss":
            swap(a)
            swap(b)

        elif op == "pa":
            push(a, b)

        elif op == "pb":
            push(b, a)

        elif op == "ra":
            rotate(a)

        elif op == "rb":
            rotate(b)

        elif op == "rr":
            rotate(a)
            rotate(b)

        elif op == "rra":
            reverse_rotate(a)

        elif op == "rrb":
            reverse_rotate(b)

        elif op == "rrr":
            reverse_rotate(a)
            reverse_rotate(b)

        else:
            return False

    return a == sorted(numbers) and not b


# ==========================================
# BENCHMARK
# ==========================================

stats = {
    "Original": [],
    "V1": [],
    "V2": [],
    "V3": [],
    "V4-A": [],
    "V4-B": [],
    "V4-C": [],
    "V4-D": [],
    "V4": [],
}

if SIZE <= 0 or TESTS <= 0:
    print("SIZE e TESTS devem ser superiores a zero")
    sys.exit(1)

limit = (
    700 if SIZE == 100
    else 5500 if SIZE == 500
    else None
)

for i in range(TESTS):
    numbers = random.sample(
        range(1, SIZE + 1), SIZE
    )

    try:
        process = subprocess.run(
            ["./push_swap", "--complex"]
            + list(map(str, numbers)),
            capture_output=True,
            text=True,
            timeout=10
        )

    except FileNotFoundError:
        print("Executavel push_swap nao encontrado")
        print("Executa make antes do benchmark")
        sys.exit(1)

    except subprocess.TimeoutExpired:
        print(f"TIMEOUT no teste {i + 1}")
        print("Input:", *numbers)
        sys.exit(1)

    if process.returncode != 0:
        print("ERRO ao executar push_swap")
        print(process.stderr)
        sys.exit(1)

    original = process.stdout.split()

    if not checker(numbers, original):
        print("\nKO no algoritmo original")
        print(f"Teste: {i + 1}")
        print("Input:", *numbers)
        sys.exit(1)

    v1 = optimize_pairs(original, combine_v1)
    v2 = optimize_pairs(original, combine_v2)
    v3 = optimize_v3(original)

    v4_a = optimize_v4_a(original, SIZE)
    v4_b = optimize_v4_b(original, SIZE)
    v4_c = optimize_v4_c(original, SIZE)
    v4_d = optimize_v4_d(original, SIZE)

    v4 = optimize_v4(original, SIZE)

    versions = {
        "Original": original,
        "V1": v1,
        "V2": v2,
        "V3": v3,
        "V4-A": v4_a,
        "V4-B": v4_b,
        "V4-C": v4_c,
        "V4-D": v4_d,
        "V4": v4,
    }

    for name, operations in versions.items():
        if not checker(numbers, operations):
            print(f"\nKO na versao {name}")
            print(f"Teste: {i + 1}")
            print("Input:", *numbers)
            print("Operacoes:", *operations)
            sys.exit(1)

        stats[name].append(len(operations))

    if (i + 1) % 100 == 0:
        print(f"Testes concluidos: {i + 1}/{TESTS}")


# ==========================================
# RESULTADOS
# ==========================================

original_avg = sum(stats["Original"]) / TESTS
v3_avg = sum(stats["V3"]) / TESTS

print("\n========== RESULTADOS ==========")
print(f"Elementos: {SIZE}")
print(f"Testes: {TESTS}")

for name, values in stats.items():
    average = sum(values) / TESTS
    saved = original_avg - average
    saved_v3 = v3_avg - average

    reduction = (
        saved / original_avg * 100
        if original_avg > 0 else 0
    )

    print(f"\n--- {name} ---")
    print(f"Media: {average:.2f}")
    print(f"Minimo: {min(values)}")
    print(f"Maximo: {max(values)}")
    print(f"Media poupada: {saved:.2f}")
    print(f"Reducao: {reduction:.2f}%")

    if name.startswith("V4"):
        print(f"Ganho adicional sobre V3: {saved_v3:.2f}")

    if limit is not None:
        success = sum(v < limit for v in values)

        print(f"Abaixo de {limit}: {success}/{TESTS}")
        print(f"Taxa de sucesso: {success / TESTS * 100:.2f}%")

    print("Checker: OK")

print("\n================================")

# ==========================================
# COMPARACAO DIRETA V4-D VS V4
# ==========================================

greedy = stats["V4-D"]
dynamic = stats["V4"]

equal = sum(
    g == d for g, d in zip(greedy, dynamic)
)

greedy_better = sum(
    g < d for g, d in zip(greedy, dynamic)
)

dynamic_better = sum(
    d < g for g, d in zip(greedy, dynamic)
)

print("\n===== GREEDY VS PROGRAMACAO DINAMICA =====")
print(f"Resultados iguais: {equal}/{TESTS}")
print(f"Greedy melhor: {greedy_better}/{TESTS}")
print(f"Programacao dinamica melhor: {dynamic_better}/{TESTS}")

difference = (
    sum(greedy) - sum(dynamic)
) / TESTS

print(f"Diferenca media (Greedy - DP): {difference:.2f}")
print("=========================================")
