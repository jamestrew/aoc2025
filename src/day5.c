#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DAY = 5;

const size_t input_parts = 2;

typedef struct {
  uint64_t start;
  uint64_t end;
} Range;

void print_ranges(const Range *ranges, size_t rng_size) {
  for (size_t i = 0; i < rng_size; ++i) {
    Range r = ranges[i];
    printf("%zu-%zu\n", r.start, r.end);
  }
}

void parse_ranges(const char *input, Range **ranges, size_t *rng_size) {
  char **lines = split(input, "\n", rng_size);
  *ranges = realloc(*ranges, *rng_size * sizeof(Range));
  size_t c = 0;

  for (size_t i = 0; i < *rng_size; ++i) {
    char **parts = split(lines[i], "-", &c);
    assert(c == 2);
    (*ranges)[i].start = strtoull(parts[0], NULL, 10);
    (*ranges)[i].end = strtoull(parts[1], NULL, 10);
    free_lines(parts, c);
  }

  free_lines(lines, *rng_size);
}

bool in_ranges(Range *ranges, size_t rng_size, uint64_t ing) {
  for (size_t i = 0; i < rng_size; ++i) {
    if (ing >= ranges[i].start && ing <= ranges[i].end)
      return true;
  }
  return false;
}

Range *coalesce_ranges(Range *ranges, size_t *rng_size) {
  Range *new_rng = malloc(sizeof(Range) * *rng_size);
  size_t new_size = 0;

  for (size_t i = 0; i < *rng_size; ++i) {
    Range test = ranges[i];
    bool add = true;
    for (size_t j = i + 1; j < *rng_size; ++j) {
      Range *comp = &(ranges[j]);

      if (test.start >= comp->start && test.start <= comp->end) {
        if (test.end > comp->end) {
          comp->end = test.end;
        }
        add = false;
      } else if (test.end >= comp->start && test.end <= comp->end) {
        if (test.start < comp->start) {
          comp->start = test.start;
        }
        add = false;
      } else if (test.start < comp->start && test.end > comp->end) {
        comp->start = test.start;
        comp->end = test.end;
        add = false;
      }
    }

    if (add) {
      new_rng[new_size].start = test.start;
      new_rng[new_size].end = test.end;
      new_size++;
    }
  }

  free(ranges);
  *rng_size = new_size;
  return new_rng;
}

void parse_ingredients(const char *input, uint64_t **ingredients, size_t *ing_count) {
  char **lines = split(input, "\n", ing_count);
  *ingredients = realloc(*ingredients, *ing_count * sizeof(uint64_t));

  for (size_t i = 0; i < *ing_count; ++i) {
    (*ingredients)[i] = strtoull(lines[i], NULL, 10);
  }

  free_lines(lines, *ing_count);
}

char **split_input_parts(const char *input) {
  char **parts = malloc(input_parts * sizeof(char *));
  for (size_t i = 0; i < strlen(input); ++i) {
    if (input[i] == '\n' && input[i + 1] == '\n') {
      parts[0] = strndup(input, i);
      parts[1] = strdup(input + i + 1);
      break;
    }
  }

  return parts;
}

void parse_input(const char *input, Range **ranges, size_t *rng_size, uint64_t **ingredients, size_t *ing_count) {
  char **parts = split_input_parts(input);
  parse_ranges(parts[0], ranges, rng_size);
  parse_ingredients(parts[1], ingredients, ing_count);
  free_lines(parts, input_parts);
}

uint64_t part1(const char *input) {
  uint64_t fresh_count = 0;

  uint64_t *ingredients = NULL;
  size_t ing_count = 0;
  Range *ranges = NULL;
  size_t rng_size = 0;
  parse_input(input, &ranges, &rng_size, &ingredients, &ing_count);

  for (size_t i = 0; i < ing_count; ++i) {
    if (in_ranges(ranges, rng_size, ingredients[i]))
      fresh_count++;
  }

  free(ingredients);
  free(ranges);
  return fresh_count;
}

uint64_t part2(const char *input) {
  char **parts = split_input_parts(input);
  Range *ranges = NULL;
  size_t rng_size = 0;
  parse_ranges(parts[0], &ranges, &rng_size);
  ranges = coalesce_ranges(ranges, &rng_size);

  uint64_t fresh_count = 0;
  for (size_t i = 0; i < rng_size; ++i) {
    fresh_count += ranges[i].end - ranges[i].start + 1;
  }

  free_lines(parts, 2);
  free(ranges);
  return fresh_count;
}

void test() {
  const char *test_input =

      "3-5\n"
      "10-14\n"
      "16-20\n"
      "12-18\n"
      "\n"
      "1\n"
      "5\n"
      "8\n"
      "11\n"
      "17\n"
      "32";

  ASSERT_EQ(3, part1(test_input));
  ASSERT_EQ(14, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  test();
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
}
