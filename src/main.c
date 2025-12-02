#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>
#include <unistd.h>
#include "util.h"

typedef void (*test_fn_t)(void);
typedef uint64_t (*solution_fn_t)(const char*);

void print_usage(const char* prog) {
    fprintf(stderr, "Usage: %s <day> [--submit <part>]\n", prog);
    fprintf(stderr, "  day:         Day number (1-25)\n");
    fprintf(stderr, "  --submit:    Submit answer for part 1 or 2\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    int day = atoi(argv[1]);
    if (day < 1 || day > 25) {
        fprintf(stderr, "Error: day must be 1-25\n");
        return 1;
    }

    int submit_flag = 0;
    int submit_part = 0;
    if (argc == 4 && strcmp(argv[2], "--submit") == 0) {
        submit_flag = 1;
        submit_part = atoi(argv[3]);
        if (submit_part != 1 && submit_part != 2) {
            fprintf(stderr, "Error: submit part must be 1 or 2\n");
            return 1;
        }
    } else if (argc > 2) {
        print_usage(argv[0]);
        return 1;
    }

    char src_path[256];
    snprintf(src_path, sizeof(src_path), "src/day%d.c", day);
    if (access(src_path, F_OK) != 0) {
        printf("Day %d not found, generating...\n", day);
        char gen_cmd[256];
        snprintf(gen_cmd, sizeof(gen_cmd), "./scripts/gen_day.sh %d", day);
        int ret = system(gen_cmd);
        if (ret != 0) {
            fprintf(stderr, "Failed to generate day %d\n", day);
            return 1;
        }
    }

    char lib_path[256];
    snprintf(lib_path, sizeof(lib_path), "bin/libday%d.so", day);

    char build_cmd[256];
    snprintf(build_cmd, sizeof(build_cmd), "make -s bin/libday%d.so", day);
    int ret = system(build_cmd);
    if (ret != 0) {
        fprintf(stderr, "Failed to build day %d\n", day);
        return 1;
    }

    void* handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to load %s: %s\n", lib_path, dlerror());
        fprintf(stderr, "Try running: make bin/libday%d.so\n", day);
        return 1;
    }

    dlerror();

    test_fn_t test_fn = (test_fn_t)dlsym(handle, "test");
    char* error = dlerror();
    if (error) {
        fprintf(stderr, "Failed to find 'test': %s\n", error);
        dlclose(handle);
        return 1;
    }

    solution_fn_t part1_fn = (solution_fn_t)dlsym(handle, "part1");
    error = dlerror();
    if (error) {
        fprintf(stderr, "Failed to find 'part1': %s\n", error);
        dlclose(handle);
        return 1;
    }

    solution_fn_t part2_fn = (solution_fn_t)dlsym(handle, "part2");
    error = dlerror();
    if (error) {
        fprintf(stderr, "Failed to find 'part2': %s\n", error);
        dlclose(handle);
        return 1;
    }

    printf("Running tests...\n");
    test_fn();

    char* input = read_day_input(day);
    if (!input) {
        fprintf(stderr, "Failed to read input for day %d\n", day);
        dlclose(handle);
        return 1;
    }

    uint64_t result1 = part1_fn(input);
    printf("Part 1: %lu\n", result1);

    uint64_t result2 = part2_fn(input);
    printf("Part 2: %lu\n", result2);

    if (submit_flag) {
        char submit_cmd[512];
        uint64_t result = (submit_part == 1) ? result1 : result2;
        snprintf(submit_cmd, sizeof(submit_cmd),
                 "./scripts/submit_answer.sh %d %d %lu",
                 day, submit_part, result);
        printf("Submitting: %s\n", submit_cmd);
        system(submit_cmd);
    }

    free(input);
    dlclose(handle);

    return 0;
}
