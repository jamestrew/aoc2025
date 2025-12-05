#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const int DAY = 4;

typedef struct {
  size_t width;
  size_t height;
  bool **paperlocs;
  bool *_datamem;
} Grid;

Grid *parse_grid(const char *input) {
  int line_count = 0;
  char **lines = split(input, "\n", &line_count);

  Grid *g = malloc(sizeof(Grid));
  g->width = strlen(lines[0]);
  g->height = line_count;
  g->paperlocs = malloc(sizeof(bool *) * g->height);
  g->_datamem = malloc(sizeof(bool) * g->height * g->width);

  for (size_t y = 0; y < g->height; ++y) {
    g->paperlocs[y] = g->_datamem + y * g->width;
    for (size_t x = 0; x < g->width; ++x) {
      g->paperlocs[y][x] = lines[y][x] == '@';
    }
  }

  free_lines(lines, line_count);
  return g;
}

void free_grid(Grid *grid) {
  free(grid->_datamem);
  free(grid->paperlocs);
  free(grid);
}

bool grid_pos_inbounds(Grid *grid, size_t x, size_t y) {
  return x < grid->width && y < grid->height;
}

int neighbor_pcount(Grid *grid, size_t x, size_t y) {
  assert(grid_pos_inbounds(grid, x, y));
  static const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
  static const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

  int count = 0;
  for (int i = 0; i < 8; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if (grid_pos_inbounds(grid, nx, ny) && grid->paperlocs[ny][nx])
      count++;
  }

  return count;
}

uint64_t part1(const char *input) {
  Grid *g = parse_grid(input);
  uint64_t res = 0;

  for (size_t y = 0; y < g->height; ++y) {
    for (size_t x = 0; x < g->width; ++x) {
      if (g->paperlocs[y][x] && neighbor_pcount(g, x, y) < 4)
        res++;
    }
  }

  free_grid(g);
  return res;
}

uint64_t part2(const char *input) {
  Grid *g = parse_grid(input);
  uint64_t res = 0;
  uint64_t last_pass;

  do {
    last_pass = res;
    for (size_t y = 0; y < g->height; ++y) {
      for (size_t x = 0; x < g->width; ++x) {
        if (g->paperlocs[y][x] && neighbor_pcount(g, x, y) < 4) {
          res++;
          g->paperlocs[y][x] = false;
        }
      }
    }
  } while (last_pass != res);

  free_grid(g);
  return res;
}

void test() {

  const char *test_input = "..@@.@@@@.\n"
                           "@@@.@.@.@@\n"
                           "@@@@@.@.@@\n"
                           "@.@@@@..@.\n"
                           "@@.@@@@.@@\n"
                           ".@@@@@@@.@\n"
                           ".@.@.@.@@@\n"
                           "@.@@@.@@@@\n"
                           ".@@@@@@@@.\n"
                           "@.@.@@@.@.";

  ASSERT_EQ(13, part1(test_input));
  ASSERT_EQ(43, part2(test_input));
  printf("All tests passed!\n");
}

int main() {
  char *input = read_day_input(DAY);
  printf("Part 1: %lu\n", part1(input));
  printf("Part 2: %lu\n", part2(input));
}
