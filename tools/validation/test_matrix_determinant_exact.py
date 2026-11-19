#!/usr/bin/env python3
"""Compare matrix-menu determinants with an arbitrary-precision oracle."""

import argparse
import random
import subprocess

INT32_MIN = -(2**31)
INT32_MAX = 2**31 - 1


def bareiss_determinant(matrix: list[list[int]]) -> int:
    """Return an exact determinant using fraction-free elimination."""
    size = len(matrix)
    work = [row[:] for row in matrix]
    sign = 1
    denominator = 1

    for pivot_index in range(size - 1):
        if work[pivot_index][pivot_index] == 0:
            swap_index = next(
                (row for row in range(pivot_index + 1, size)
                 if work[row][pivot_index] != 0),
                None,
            )
            if swap_index is None:
                return 0
            work[pivot_index], work[swap_index] = (
                work[swap_index], work[pivot_index]
            )
            sign = -sign

        pivot = work[pivot_index][pivot_index]
        for row in range(pivot_index + 1, size):
            for column in range(pivot_index + 1, size):
                numerator = (work[row][column] * pivot
                             - work[row][pivot_index]
                             * work[pivot_index][column])
                quotient, remainder = divmod(numerator, denominator)
                if remainder != 0:
                    raise ArithmeticError("Bareiss division was not exact")
                work[row][column] = quotient
            work[row][pivot_index] = 0
        denominator = pivot

    return sign * work[size - 1][size - 1]


def test_matrices() -> list[list[list[int]]]:
    matrices = []
    for size in range(1, 9):
        matrices.append([[INT32_MIN if row == col else 0
                          for col in range(size)] for row in range(size)])
        matrices.append([[INT32_MAX if row == col else 0
                          for col in range(size)] for row in range(size)])
        matrices.append([[1 if row == col else 0 for col in range(size)]
                         for row in range(size)])
        if size > 1:
            row_swapped = [[1 if row == col else 0 for col in range(size)]
                           for row in range(size)]
            row_swapped[0], row_swapped[1] = row_swapped[1], row_swapped[0]
            matrices.append(row_swapped)
            singular = [[1 if row == col else 0 for col in range(size)]
                        for row in range(size)]
            singular[-1] = singular[-2][:]
            matrices.append(singular)

    # The 2x2 block has determinant 1, though float64 elimination can round
    # its large products to the same value. The identity tail preserves 1.
    rounding_case = [
        [2147483646, 2147483645],
        [2147483647, 2147483646],
    ]
    for size in range(2, 9):
        extended = [[0] * size for _ in range(size)]
        for row in range(2):
            extended[row][:2] = rounding_case[row]
        for diagonal in range(2, size):
            extended[diagonal][diagonal] = 1
        matrices.append(extended)

    generator = random.Random(0xD37E_A11)
    for size in range(1, 9):
        # Include dedicated dense full-range cases for sizes 5, 6, and 7.
        count = 4 if size in (5, 6, 7) else 3
        for _ in range(count):
            matrices.append([
                [generator.randint(INT32_MIN, INT32_MAX)
                 for _ in range(size)]
                for _ in range(size)
            ])
    return matrices


def run(executable: str) -> None:
    matrices = test_matrices()
    expected = [str(bareiss_determinant(matrix)) for matrix in matrices]
    commands = []
    for index, matrix in enumerate(matrices):
        size = len(matrix)
        commands.extend([
            "1", f"matrix_{index}", str(size), str(size),
            *(str(value) for row in matrix for value in row),
            "5", str(index + 1),
        ])
    commands.extend(["0", ""])

    try:
        result = subprocess.run(
            [executable], input="\n".join(commands), text=True,
            capture_output=True, check=False, timeout=10,
        )
    except subprocess.TimeoutExpired as error:
        raise SystemExit("matrix determinant integration timed out") from error

    if result.returncode != 0:
        raise SystemExit(
            f"matrix menu exited {result.returncode}: {result.stderr}"
        )
    actual = result.stdout.splitlines()
    # Each determinant is emitted as a standalone decimal line. Restricting
    # candidates to exact integer syntax avoids any floating-point conversion.
    determinant_lines = [line for line in actual
                         if line and (line.isdecimal()
                                      or (line[0] == "-"
                                          and line[1:].isdecimal()))]
    if determinant_lines != expected:
        for index, (want, got) in enumerate(zip(expected, determinant_lines)):
            if want != got:
                raise SystemExit(
                    f"case {index} ({len(matrices[index])}x"
                    f"{len(matrices[index])}): expected {want}, got {got}"
                )
        raise SystemExit(
            f"expected {len(expected)} determinant lines, got "
            f"{len(determinant_lines)}"
        )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()
    run(args.executable)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
