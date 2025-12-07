#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DAY = 6;

typedef struct {
  char operator;
  int *operands;
  size_t operand_size;
} Equation;

void debug_equations(const Equation *eqs, size_t count) {
  for (size_t i = 0; i < count; i++) {
    const Equation *e = &eqs[i];

    printf("Equation %zu:\n", i);
    printf("  operator: %c\n", e->operator);
    printf("  operands (%zu): ", e->operand_size);

    if (e->operands == NULL) {
      printf("(null)\n");
    } else {
      for (size_t j = 0; j < e->operand_size; j++) {
        printf("%d", e->operands[j]);
        if (j + 1 < e->operand_size)
          printf(", ");
      }
      printf("\n");
    }
  }
}

void free_equations(Equation *eqs, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    free(eqs[i].operands);
  }
  free(eqs);
}

Equation *part1_parse_input(const char *input, size_t *count) {
  size_t line_count = 0;
  char **lines = split(input, "\n", &line_count);
  char **first_parts = split(lines[0], " ", count);

  Equation *eqs = malloc(*count * sizeof(Equation));
  size_t operands = line_count - 1;

  for (size_t i = 0; i < *count; ++i) {
    eqs[i].operands = malloc(operands * sizeof(int));
    eqs[i].operands[0] = atoi(first_parts[i]);
    eqs[i].operand_size = operands;
  }
  free_lines(first_parts, *count);

  for (size_t i = 1; i < line_count - 1; ++i) {
    char **ops = split(lines[i], " ", count);
    for (size_t j = 0; j < *count; ++j) {
      eqs[j].operands[i] = atoi(ops[j]);
    }
    free_lines(ops, *count);
  }

  char **operators = split(lines[line_count - 1], " ", count);
  for (size_t i = 0; i < *count; ++i) {
    eqs[i].operator = operators[i][0];
  }
  free_lines(operators, *count);

  free_lines(lines, line_count);
  return eqs;
}

uint64_t part1(const char *input) {
  size_t count = 0;
  Equation *eqs = part1_parse_input(input, &count);

  uint64_t sum = 0;

  for (size_t i = 0; i < count; ++i) {
    Equation eq = eqs[i];
    uint64_t ans = eq.operator == '+' ? 0 : 1;

    for (size_t j = 0; j < eq.operand_size; ++j) {
      if (eq.operator == '+')
        ans += (uint64_t)eq.operands[j];
      else
        ans *= (uint64_t)eq.operands[j];
    }
    sum += ans;
  }

  free_equations(eqs, count);
  return sum;
}

Equation *get_ops(char *opline, size_t *eqcount) {
  char **tokens = split(opline, " ", eqcount);
  free_lines(tokens, *eqcount);

  Equation *eqs = malloc(*eqcount * sizeof(Equation));
  size_t p = 0;

  for (size_t eqidx = 0; eqidx < *eqcount; ++eqidx) {
    Equation *eq = &eqs[eqidx];
    eq->operator = opline[p];
    size_t opcount = 1;

    do {
      p++;
      if (opline[p] == ' ')
        opcount++;
    } while (opline[p] == ' ');

    eq->operand_size = (eqidx == *eqcount - 1) ? opcount : opcount - 1;
    eq->operands = malloc(eq->operand_size * sizeof(int));
  }

  return eqs;
}

void get_operands(char **lines, size_t line_count, Equation *eqs, size_t eqcount) {
  size_t p = 0;

  for (size_t eqidx = 0; eqidx < eqcount; ++eqidx) {
    Equation *eq = &(eqs[eqidx]);

    for (size_t opidx = 0; opidx < eq->operand_size; ++opidx) {
      char buf[32];
      char digit = 0;
      size_t digits = 0;
      for (size_t lidx = 0; lidx < line_count - 1; ++lidx) {
        digit = lines[lidx][p];
        if (digit == ' ') {
          continue;
        }
        buf[digits] = digit;
        digits++;
      }
      buf[digits] = '\0';
      eq->operands[opidx] = atoi(buf);
      p++;
    }
    p++; // whitespace between cols
  }
}

Equation *part2_parse_input(const char *input, size_t *count) {
  size_t line_count = 0;
  char **lines = split(input, "\n", &line_count);

  Equation *eqs = get_ops(lines[line_count - 1], count);
  get_operands(lines, line_count, eqs, *count);
  debug_equations(eqs, *count);
  return eqs;
}

uint64_t part2(const char *input) {
  size_t count = 0;
  Equation *eqs = part2_parse_input(input, &count);

  uint64_t sum = 0;

  for (size_t i = 0; i < count; ++i) {
    Equation eq = eqs[i];
    uint64_t ans = eq.operator == '+' ? 0 : 1;

    for (size_t j = 0; j < eq.operand_size; ++j) {
      if (eq.operator == '+')
        ans += (uint64_t)eq.operands[j];
      else
        ans *= (uint64_t)eq.operands[j];
    }
    sum += ans;
  }

  free_equations(eqs, count);
  return sum;
}

void test() {
  const char *test_input =

      "123 328  51 64 \n"
      " 45 64  387 23 \n"
      "  6 98  215 314\n"
      "*   +   *   +  ";

  ASSERT_EQ(4277556, part1(test_input));
  ASSERT_EQ(3263827, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  test();
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
  free(input);
}
