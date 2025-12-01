#ifndef UTIL_H
#define UTIL_H

char *read_day_input(int day);
char **read_day_lines(int day, int *line_count);

void trim(char *str);
int split(char *str, const char *delim, char **tokens, int max_tokens);

void free_lines(char **lines, int count);

#endif
