#!/usr/bin/env bash

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <day_number>" >&2
    exit 1
fi

DAY=$1

if [ ! -f .env ]; then
    echo "Error: .env file not found" >&2
    exit 1
fi

AOC_SESSION=$(grep "^AOC_SESSION=" .env | cut -d'=' -f2)
AOC_YEAR=$(grep "^AOC_YEAR=" .env | cut -d'=' -f2)

if [ -z "$AOC_SESSION" ]; then
    echo "Error: AOC_SESSION not found in .env" >&2
    exit 1
fi

if [ -z "$AOC_YEAR" ]; then
    echo "Error: AOC_YEAR not found in .env" >&2
    exit 1
fi

if [ "$AOC_YEAR" -lt 2015 ]; then
    echo "Error: Invalid year $AOC_YEAR" >&2
    exit 1
fi

if [ "$DAY" -lt 1 ] || [ "$DAY" -gt 25 ]; then
    echo "Error: Invalid day $DAY (must be 1-25)" >&2
    exit 1
fi

mkdir -p inputs

CACHE_FILE="inputs/day${DAY}"

if [ -f "$CACHE_FILE" ]; then
    echo "Input for day $DAY already cached"
    exit 0
fi

echo "Fetching input for day $DAY..."

if ! curl -f -s -H "Cookie: session=$AOC_SESSION" \
    "https://adventofcode.com/${AOC_YEAR}/day/${DAY}/input" \
    -o "$CACHE_FILE"; then
    echo "Error: Failed to fetch input from adventofcode.com" >&2
    rm -f "$CACHE_FILE"
    exit 1
fi

echo "Input cached to $CACHE_FILE"
