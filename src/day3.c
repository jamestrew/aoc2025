#include "util.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t part1(const char *input) {
  int line_count = 0;
  char **lines = split(input, "\n", &line_count);
  uint64_t sum = 0;

  for (int i = 0; i < line_count; ++i) {
    char *line = lines[i];

    int num = 0;
    size_t digitpos = 0;
    char digit1 = 0;
    for (size_t j = 0; j < strlen(line) - 1; ++j) {
      if (line[j] > digit1) {
        digitpos = j;
        digit1 = line[j];
      }
    }

    char digit2 = 0;
    for (size_t j = digitpos + 1; j < strlen(line); ++j) {
      if (line[j] > digit2) {
        digit2 = line[j];
      }
    }

    sum += (uint64_t)numfromchars(2, digit1, digit2);
  }

  free_lines(lines, line_count);
  return sum;
}

uint64_t part2(const char *input) { return 0; }

void test() {
  const char *test_input = "987654321111111\n"
                           "811111111111119\n"
                           "234234234234278\n"
                           "818181911112111";
  ASSERT_EQ(357, part1(test_input));
  /* ASSERT_EQ(3121910778619, part2(test_input)); */
  printf("All tests passed!\n");
}
