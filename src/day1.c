#include "util.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int DIAL_MIN = 0;
const int DIAL_MAX = 99;
const int DIAL_POSITIONS = 100;

int *parse_instruction(const char *input, int *count) {
  char **lines = split(input, "\n", count);

  int *instructions = malloc(*count * sizeof(int));
  assert(instructions);

  for (int i = 0; i < *count; ++i) {
    char *line = lines[i];

    int dir;
    if (line[0] == 'L')
      dir = -1;
    else if (line[0] == 'R')
      dir = 1;
    else {
      fprintf(stderr, "expected L##/R## from line. got %s", line);
      abort();
    }

    int clicks = strtol(line + 1, NULL, 10);
    instructions[i] = dir * clicks;
  }

  free_lines(lines, *count);
  return instructions;
}


uint64_t part1(const char *input) {
  int instr_count = 0;
  int *instructions = parse_instruction(input, &instr_count);

  int curr_num = 50;
  int zeroed_count = 0;

  for (int i = 0; i < instr_count; ++i) {
    int clicks = instructions[i];
    curr_num = (curr_num + clicks) % DIAL_POSITIONS;
    if (curr_num < DIAL_MIN)
      curr_num += DIAL_POSITIONS;

    if (curr_num == 0)
      zeroed_count++;
  }

  free(instructions);
  return zeroed_count;
}

uint64_t part2(const char *input) {
  int instr_count = 0;
  int *instructions = parse_instruction(input, &instr_count);

  int curr_pos = 50;
  int zeroed_count = 0;

  for (int i = 0; i < instr_count; ++i) {
    int clicks = instructions[i];
    if (clicks == 0)
      continue;

    int end_pos = curr_pos + clicks;

    if (clicks > 0) {
      zeroed_count += floor_div(end_pos, DIAL_POSITIONS) -
                      floor_div(curr_pos, DIAL_POSITIONS);
    } else {
      zeroed_count += floor_div(curr_pos - 1, DIAL_POSITIONS) -
                      floor_div(end_pos - 1, DIAL_POSITIONS);
    }

    curr_pos = end_pos;
  }

  free(instructions);
  return zeroed_count;
}

void test() {
  const char *test_input = "L68\n"
                           "L30\n"
                           "R48\n"
                           "L5\n"
                           "R60\n"
                           "L55\n"
                           "L1\n"
                           "L99\n"
                           "R14\n"
                           "L82";

  ASSERT_EQ(3, part1(test_input));
  ASSERT_EQ(6, part2(test_input));
  printf("All tests passed!\n");
}
