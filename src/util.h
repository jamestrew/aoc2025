#ifndef UTIL_H
#define UTIL_H

char *read_day_input(int day);
char **read_day_lines(int day, int *line_count);

void trim(char *str);
char **split(const char *str, const char *delim, int *count);

void free_lines(char **lines, int count);

#endif
