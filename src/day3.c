#include "util.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t joltage_sum(const char *input, int battery_count) {
  int line_count = 0;
  char **lines = split(input, "\n", &line_count);
  uint64_t sum = 0;

  for (int i = 0; i < line_count; ++i) {
    char *bank = lines[i];
    size_t bank_size = strlen(bank);

    uint64_t joltage = 0;
    int search_start_idx = 0;

    for (int bidx = 0; bidx < battery_count; ++bidx) {
      int search_end_idx = bank_size - battery_count + bidx;
      char maxdigit = 0;
      for (int i = search_start_idx; i <= search_end_idx; ++i) {
        if (bank[i] > maxdigit) {
          maxdigit = bank[i];
          search_start_idx = i;
        }
      }

      joltage = (joltage * 10) + (maxdigit - '0');
      search_start_idx++;
    }

    sum += joltage;
  }

  free_lines(lines, line_count);
  return sum;
}

uint64_t part1(const char *input) { return joltage_sum(input, 2); }

uint64_t part2(const char *input) { return joltage_sum(input, 12); }

void test() {
  const char *test_input = "987654321111111\n"
                           "811111111111119\n"
                           "234234234234278\n"
                           "818181911112111";
  ASSERT_EQ(357, part1(test_input));
  ASSERT_EQ(3121910778619, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  char *input = read_day_input(3);

  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
}
