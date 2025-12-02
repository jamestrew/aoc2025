#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int UNLIMITED_REPEATS = 20; // max 20 digits

typedef struct {
  char **lines;
  int line_count;
  int line_curr;
  uint64_t rng_curr;
  uint64_t rng_end;
} IdIter;

void _id_iter_read_range(IdIter *iditer) {
  char *line = iditer->lines[iditer->line_curr];
  int c = 0;
  char **ranges = split(line, "-", &c);
  assert(c == 2);
  iditer->rng_curr = strtoull(ranges[0], NULL, 10);
  iditer->rng_end = strtoull(ranges[1], NULL, 10);
  free_lines(ranges, c);
}

void id_iter_init(const char *input, IdIter *iditer) {
  iditer->line_count = 0;
  iditer->lines = split(input, ",", &iditer->line_count);
  iditer->line_curr = 0;

  _id_iter_read_range(iditer);
}

bool id_iter_next(IdIter *iditer, uint64_t *v) {
  *v = iditer->rng_curr;

  if (iditer->rng_curr <= iditer->rng_end) {
    iditer->rng_curr++;
    return true;
  }

  if (iditer->line_curr == iditer->line_count - 1)
    return false; // fully drained

  iditer->line_curr++;
  _id_iter_read_range(iditer);

  return true;
}

void id_iter_free(IdIter *iditer) {
  if (iditer->lines) {
    free_lines(iditer->lines, iditer->line_count);
    iditer->lines = NULL;
  }
}

bool is_invalid(uint64_t v, int max_repeats) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%lu", v);
  int len = strlen(buf);

  for (int cmplen = len / 2; cmplen > 0; --cmplen) {
    if (len / cmplen > max_repeats)
      break;
    if (len % cmplen != 0)
      continue;

    bool good = true;
    for (int start = cmplen; start < len; start += cmplen) {
      if (strncmp(buf, buf + start, cmplen) != 0) {
        good = false;
        break;
      }
    }
    if (good)
      return true;
  }

  return false;
}

uint64_t invalid_id_sum(const char *input, int max_repeats) {
  uint64_t sum = 0;
  IdIter iter;
  id_iter_init(input, &iter);

  uint64_t num;
  while (id_iter_next(&iter, &num)) {
    if (is_invalid(num, max_repeats)) {
      sum += num;
    }
  }

  id_iter_free(&iter);
  return sum;
}

uint64_t part1(const char *input) { return invalid_id_sum(input, 2); }

uint64_t part2(const char *input) {
  return invalid_id_sum(input, UNLIMITED_REPEATS);
}

void test() {
  const char *test_input =
      "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-"
      "1698528,446443-446449,38593856-38593862,565653-565659,824824821-"
      "824824827,2121212118-2121212124";
  ASSERT_EQ(1227775554, part1(test_input));
  ASSERT_TRUE(is_invalid(1212121212, UNLIMITED_REPEATS));
  ASSERT_EQ(33, invalid_id_sum("11-22", 2));
  ASSERT_EQ(4174379265, part2(test_input));
  printf("All tests passed!\n");
}
