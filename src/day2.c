#include "util.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t part1(const char *input) {
  int line_count = 0;
  char **lines = split(input, ",", &line_count);

  uint64_t sum = 0;

  for (int i = 0; i < line_count; ++i) {
    int c = 0;
    char **ranges = split(lines[i], "-", &c);
    assert(c == 2);
    uint64_t start = strtoull(ranges[0], NULL, 10);
    uint64_t end  = strtoull(ranges[1], NULL, 10);

    for (uint64_t x = start; x <= end; ++x) {
      char buf[20];
      snprintf(buf, sizeof(buf), "%lu", x);
      int len = strlen(buf);
      if (len % 2 != 0)
        continue;
      if (strncmp(buf, buf + (len / 2), len / 2) == 0) {
        sum += x;
      }
    }
  }
  return sum;
}

uint64_t part2(const char *input) { return 0; }

void test() {
  const char *test_input =
      "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-"
      "1698528,446443-446449,38593856-38593862,565653-565659,824824821-"
      "824824827,2121212118-2121212124";
  ASSERT_EQ(1227775554, part1(test_input));
  ASSERT_EQ(0, part2(test_input));
  printf("All tests passed!\n");
}
