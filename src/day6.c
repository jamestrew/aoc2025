#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int DAY = 6;

typedef struct {
  char operator;
  int *operand;
  size_t operand_size;
} Equation;

// parsing...
// split by lines
// count of lines = count of operands + 1 for operator

size_t equation_count(const char *line) {
  size_t count = 0;
  char **parts = split(line, " ", &count);

  for (size_t i = 0; i < count; ++i) {
    printf("'%s' ", parts[i]);
  }

  printf("\n");
  printf("count: %zu\n", count);
  return count;
}

Equation *parse_input(const char *input, size_t *count) {
  size_t line_count = 0;
  char **lines = split(input, "\n", &line_count);
  char **first_parts = split(lines[0], " ", count);

  Equation *eqs = malloc(*count * sizeof(Equation));
  size_t operands = line_count - 1;

  for (size_t i = 0; i < *count; ++i) {
    Equation *eq = malloc(sizeof(Equation));
    eq->operand = malloc(operands * sizeof(int));
    eq->operand[0] = atoi(first_parts[i]);
    eq->operand_size = operands;
    eqs[i] = *eq;
  }
  free_lines(first_parts, *count);

  for (size_t i = 1; i < line_count - 1; ++i) {
    char **ops = split(lines[i], " ", count);
    for (size_t j = 0; j < *count; ++j) {
      eqs[j].operand[i] = atoi(ops[j]);
    }
    free_lines(ops, *count);
  }

  char **operators = split(lines[line_count - 1], " ", count);
  for (size_t i = 0; i < *count; ++i) {
    eqs[i].operator = operators[i][0];
  }
  free_lines(operators, *count);

  /* free_lines(lines + 1, *count); */
  return eqs;
}

uint64_t part1(const char *input) {
  size_t count = 0;
  Equation *eqs = parse_input(input, &count);

  uint64_t sum = 0;

  for (size_t i = 0; i < count; ++i) {
    Equation eq = eqs[i];
    uint64_t ans = eq.operator == '+' ? 0 : 1;

    for (size_t j = 0; j < eq.operand_size; ++j) {
      printf("%d %c ", eq.operand[j], j == eq.operand_size - 1 ? '\0' : eq.operator);
      if (eq.operator == '+')
        ans += (uint64_t)eq.operand[j];
      else
        ans *= (uint64_t)eq.operand[j];
    }

    printf("= %lu\n", ans);
    sum += ans;
  }


  // TODO: free equations
  return sum;
}

uint64_t part2(const char *input) { return 0; }

void test() {
  const char *test_input =

      "123 328  51 64 \n"
      " 45 64  387 23 \n"
      "  6 98  215 314\n"
      "*   +   *   +  ";

  ASSERT_EQ(4277556, part1(test_input));
  ASSERT_EQ(0, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  test();
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
}
