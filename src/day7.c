#include "util.h"
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
  m->cells = malloc(sizeof(CellKind *) * rows);
  m->_cellsmem = malloc(sizeof(CellKind) * rows * cols);
  for (size_t row = 0; row < m->rows; ++row) {
    m->cells[row] = m->_cellsmem + row * m->cols;
  }
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
  if (col < 0 || col >= b->_capacity)
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
      if (m->cells[row][beam_col] == EMPTY) {
        add_beams(next_beams, beam_col);
      } else if (m->cells[row][beam_col] == SPLITTER) {
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

uint64_t part2(const char *input) { return 0; }

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
