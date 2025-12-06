#include "util.h"
#include <stdint.h>
#include <stdio.h>

const int DAY = 0;

uint64_t part1(const char *input) { return 0; }

uint64_t part2(const char *input) { return 0; }

void test() {
  const char *test_input = "test data here";
  ASSERT_EQ(0, part1(test_input));
  ASSERT_EQ(0, part2(test_input));
  printf("All tests passed!\n");
}


int main() {
  test();
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
  free(input);
}
