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
    for (size_t l = 0; l < strlen(line); ++l) {
      for (size_t r = l + 1; r < strlen(line); ++r) {
        char buf[3];
        int num1 = line[l] - '0';
        int num2 = line[r] - '0';
        snprintf(buf, sizeof(buf), "%d%d", num1, num2);
        int x = atoi(buf);
        num = max(num, x);
      }
    }

    sum += (uint64_t)num;
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
  ASSERT_EQ(3121910778619, part2(test_input));
  printf("All tests passed!\n");
}
