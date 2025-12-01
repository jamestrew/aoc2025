#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "util.h"

int part1(const char* input) {
    return 0;
}

int part2(const char* input) {
    return 0;
}

void test() {
    const char* test_input = "test data here";
    assert(part1(test_input) == 0);
    assert(part2(test_input) == 0);
    printf("All tests passed!\n");
}

int main() {
    test();

    char* input = read_day_input(0);
    if (!input) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }

    printf("Part 1: %d\n", part1(input));
    printf("Part 2: %d\n", part2(input));

    free(input);
    return 0;
}
