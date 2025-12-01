# Advent of Code 2025

C solutions for Advent of Code 2025 with automated input fetching and boilerplate generation.

## Setup

### Environment Variables

Create `.env` file in project root:

```bash
AOC_SESSION=your_session_cookie_here
AOC_YEAR=2025
```

### Finding Session Cookie

1. Login to [adventofcode.com](https://adventofcode.com)
2. Open browser dev tools (F12)
3. Go to Application/Storage -> Cookies -> https://adventofcode.com
4. Copy value of `session` cookie
5. Paste into `.env` as `AOC_SESSION`

## Quick Start

Generate code + fetch input for day N:

```bash
make gen1   # day 1
make gen15  # day 15
```

Run a day's solution:

```bash
make run1   # builds and runs day 1
make run15  # builds and runs day 15
```

## Structure

```
src/        Solution files (day1.c, day2.c, etc.) + utilities
inputs/     Puzzle input files (auto-fetched)
bin/        Compiled executables (git-ignored)
build/      Object files (git-ignored)
scripts/    Generation and fetch automation
```

## Workflow

1. `make gen1` - creates src/day1.c, fetches inputs/day1
2. Edit src/day1.c - implement part1() and part2()
3. `make run1` - test + run solution
