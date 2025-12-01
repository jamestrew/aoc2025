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
3. Go to Application/Storage -> Cookies -> <https://adventofcode.com>
4. Copy value of `session` cookie
5. Paste into `.env` as `AOC_SESSION`

## Usage

Build main executable:

```bash
make
```

Run a day (auto-generates + builds if needed):

```bash
./aoc <day>              # generate data/solution file, run tests + both parts
./aoc <day> --submit <part>  # submit answer for part 1 or 2
```

## Structure

```
src/        Solution files (day1.c, day2.c, etc.) + utilities
inputs/     Puzzle input files (auto-fetched)
bin/        Compiled shared libraries (git-ignored)
build/      Object files (git-ignored)
scripts/    Generation and fetch automation
```

## Workflow

1. `./aoc 1` - auto-generates src/day1.c, fetches input, builds, runs
2. Edit src/day1.c - implement part1()
3. `./aoc 1` - rebuild + run
4. `./aoc 1 --submit 1` - submit part 1 answer
5. Edit src/day1.c - implement part2()
6. `./aoc 1 --submit 2` - submit part 2 answer
