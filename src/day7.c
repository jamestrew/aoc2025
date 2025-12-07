#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

const int DAY = 7;

typedef enum { START, EMPTY, SPLITTER } CellKind;

CellKind from_char(char c) {
  switch (c) {
  case 'S':
    return START;
  case '^':
    return SPLITTER;
  case '.':
    return EMPTY;
  default:
    fprintf(stderr, "Unknown cell character: %c\n", c);
    exit(1);
  }
}

typedef struct {
  size_t rows;
  size_t cols;
  CellKind **cells;
  CellKind *_cellsmem;
} Manifold;

Manifold *new_manifold(size_t rows, size_t cols) {
  Manifold *m = malloc(sizeof(Manifold));
  m->rows = rows;
  m->cols = cols;
  alloc_flat(CellKind, m->rows, m->cols, m->cells, m->_cellsmem);
  return m;
}

size_t get_start_col(Manifold *m) {
  for (size_t col = 0; col < m->cols; ++col) {
    if (m->cells[0][col] == START)
      return col;
  }
  fprintf(stderr, "No start position found\n");
  exit(1);
}

void free_manifold(Manifold *m) {
  free(m->cells);
  free(m->_cellsmem);
  free(m);
}

typedef struct {
  size_t *cols;
  size_t count;
  size_t _capacity;
} RowBeams;

RowBeams *new_beams(Manifold *m) {
  RowBeams *beams = malloc(sizeof(RowBeams));
  beams->_capacity = m->cols;
  beams->cols = malloc(m->cols * sizeof(size_t));
  beams->count = 0;
  return beams;
}

void add_beams(RowBeams *b, size_t col) {
  if (col >= b->_capacity)
    return;
  bool exists = false;
  for (size_t i = 0; i < b->count; ++i) {
    if (b->cols[i] == col) {
      exists = true;
      break;
    }
  }

  if (!exists) {
    b->cols[b->count++] = col;
  }
}

void free_beams(RowBeams *b) {
  free(b->cols);
  free(b);
}

Manifold *parse_input(const char *input) {
  size_t line_count = 0;
  char **lines = split(input, "\n", &line_count);
  Manifold *m = new_manifold(line_count, strlen(lines[0]));

  for (size_t row = 0; row < m->rows; ++row) {
    for (size_t col = 0; col < m->cols; ++col) {
      m->cells[row][col] = from_char(lines[row][col]);
    }
  }

  free_lines(lines, line_count);
  return m;
}

uint64_t part1(const char *input) {
  Manifold *m = parse_input(input);
  uint64_t splits = 0;

  size_t col = get_start_col(m);
  RowBeams *active_beams = new_beams(m);
  add_beams(active_beams, col);

  for (size_t row = 1; row < m->rows; ++row) {
    RowBeams *next_beams = new_beams(m);
    for (size_t bidx = 0; bidx < active_beams->count; ++bidx) {
      size_t beam_col = active_beams->cols[bidx];
      CellKind cell = m->cells[row][beam_col];
      if (cell == EMPTY) {
        add_beams(next_beams, beam_col);
      } else if (cell == SPLITTER) {
        splits++;
        add_beams(next_beams, beam_col - 1);
        add_beams(next_beams, beam_col + 1);
      }
    }

    free_beams(active_beams);
    active_beams = next_beams;
  }

  free_manifold(m);
  free_beams(active_beams);
  return splits;
}

uint64_t timelines_from(Manifold *m, size_t row, size_t col, uint64_t **memo, bool **seen) {
  if (col >= m->cols)
    return 0;
  if (row == m->rows)
    return 1;
  if (seen[row][col])
    return memo[row][col];

  uint64_t res = 0;
  CellKind cell = m->cells[row][col];
  if (cell == EMPTY)
    res = timelines_from(m, row + 1, col, memo, seen);
  if (cell == SPLITTER) {
    uint64_t left = (col > 0) ? timelines_from(m, row + 1, col - 1, memo, seen) : 0;
    uint64_t right = (col + 1 < m->cols) ? timelines_from(m, row + 1, col + 1, memo, seen) : 0;
    res =  left + right;
  }

  memo[row][col] = res;
  seen[row][col] = true;
  return res;
}

uint64_t part2_recursive_memo(const char *input) {
  Manifold *m = parse_input(input);
  size_t col = get_start_col(m);

  uint64_t **memo;
  uint64_t *memo_flat;
  alloc_flat(uint64_t, m->rows, m->cols, memo, memo_flat);

  bool **seen;
  bool *seen_flat;
  alloc_flat(bool, m->rows, m->cols, seen, seen_flat);

  uint64_t ans = timelines_from(m, 1, col, memo, seen);
  free_manifold(m);
  free(seen_flat);
  free(memo_flat);
  free(memo);
  free(seen);
  return ans;
}

uint64_t part2_dp(const char *input) {
  Manifold *m = parse_input(input);
  size_t col = get_start_col(m);

  uint64_t *current = calloc(m->cols, sizeof(uint64_t));
  uint64_t *next = calloc(m->cols, sizeof(uint64_t));
  current[col] = 1;

  for (size_t row = 1; row < m->rows; ++row) {
    memset(next, 0, m->cols * sizeof(uint64_t));

    for (size_t col = 0; col < m->cols; ++col) {
      uint64_t cnt = current[col];
      if (cnt == 0)
        continue;

      CellKind cell = m->cells[row][col];
      if (cell == EMPTY) {
        next[col] += cnt;
      } else if (cell == SPLITTER) {
        if (col > 0) {
          next[col - 1] += cnt;
        }
        if (col + 1 < m->cols) {
          next[col + 1] += cnt;
        }
      }
    }

    uint64_t *tmp = current;
    current = next;
    next = tmp;
  }

  uint64_t total = 0;
  for (size_t col = 0; col < m->cols; ++col) {
    total += current[col];
  }

  free(current);
  free(next);
  free_manifold(m);
  return total;
}

uint64_t part2(const char *input) {
  return part2_dp(input);
  /* return part2_recursive_memo(input); */
}

void test() {
  const char *test_input = ".......S.......\n"
                           "...............\n"
                           ".......^.......\n"
                           "...............\n"
                           "......^.^......\n"
                           "...............\n"
                           ".....^.^.^.....\n"
                           "...............\n"
                           "....^.^...^....\n"
                           "...............\n"
                           "...^.^...^.^...\n"
                           "...............\n"
                           "..^...^.....^..\n"
                           "...............\n"
                           ".^.^.^.^.^...^.\n"
                           "...............";

  ASSERT_EQ(21, part1(test_input));
  ASSERT_EQ(40, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  test();
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
  free(input);
}
