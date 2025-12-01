#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file_internal(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return NULL;

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = malloc(size + 1);
  if (!buffer) {
    fclose(f);
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, size, f);
  buffer[bytes_read] = '\0';

  fclose(f);
  return buffer;
}

char *read_day_input(int day) {
  if (day < 1 || day > 25) {
    fprintf(stderr, "Error: Invalid day %d (must be 1-25)\n", day);
    return NULL;
  }

  char cache_path[256];
  snprintf(cache_path, sizeof(cache_path), "inputs/day%d", day);

  char *content = read_file_internal(cache_path);
  if (!content) {
    fprintf(stderr, "Error: Could not read %s\n", cache_path);
    fprintf(stderr, "Run: ./scripts/fetch_input.sh %d\n", day);
    return NULL;
  }

  trim(content);
  return content;
}

char **read_day_lines(int day, int *line_count) {
  char *content = read_day_input(day);
  if (!content) {
    *line_count = 0;
    return NULL;
  }

  int capacity = 16;
  char **lines = malloc(capacity * sizeof(char *));
  *line_count = 0;

  char *line = strtok(content, "\n");
  while (line) {
    if (*line_count >= capacity) {
      capacity *= 2;
      lines = realloc(lines, capacity * sizeof(char *));
    }
    lines[*line_count] = strdup(line);
    (*line_count)++;
    line = strtok(NULL, "\n");
  }

  free(content);
  return lines;
}

void trim(char *str) {
  char *start = str;
  while (isspace(*start))
    start++;

  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }

  char *end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) {
    *end = '\0';
    end--;
  }
}

int split(char *str, const char *delim, char **tokens, int max_tokens) {
  int count = 0;
  char *token = strtok(str, delim);

  while (token && count < max_tokens) {
    tokens[count++] = token;
    token = strtok(NULL, delim);
  }

  return count;
}

void free_lines(char **lines, int count) {
  for (int i = 0; i < count; i++) {
    free(lines[i]);
  }
  free(lines);
}
