#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_day_input(int day);
char **read_day_lines(int day, int *line_count);

void trim(char *str);
char **split(const char *str, const char *delim, int *count);

void free_lines(char **lines, int count);

int min(int a, int b);
int max(int a, int b);
int clamp(int x, int lo, int hi);
int floor_div(int a, int b);

#define ASSERT_EQ(expected, actual) \
  do { \
    long long exp = (long long)(expected); \
    long long act = (long long)(actual); \
    if (exp != act) { \
      fprintf(stderr, "%s:%d: ASSERT_EQ failed: expected %lld, got %lld\n", \
              __FILE__, __LINE__, exp, act); \
      exit(1); \
    } \
  } while(0)

#define ASSERT_STR_EQ(expected, actual) \
  do { \
    const char *exp = (expected); \
    const char *act = (actual); \
    if (strcmp(exp, act) != 0) { \
      fprintf(stderr, "%s:%d: ASSERT_STR_EQ failed: expected \"%s\", got \"%s\"\n", \
              __FILE__, __LINE__, exp, act); \
      exit(1); \
    } \
  } while(0)

#define ASSERT_TRUE(expr) \
  do { \
    if (!(expr)) { \
      fprintf(stderr, "%s:%d: ASSERT_TRUE failed: %s\n", \
              __FILE__, __LINE__, #expr); \
      exit(1); \
    } \
  } while(0)

#endif
