#!/usr/bin/env bash

if [ -z "$1" ]; then
    echo "Usage: $0 <day_number>"
    exit 1
fi

DAY=$1
OUTPUT_FILE="src/day${DAY}.c"

if [ -f "$OUTPUT_FILE" ]; then
    echo "Error: $OUTPUT_FILE already exists"
    exit 1
fi

sed "s/read_day_input(0)/read_day_input($DAY)/" src/day0.c > "$OUTPUT_FILE"

echo "Generated $OUTPUT_FILE"

./scripts/fetch_input.sh "$DAY" || true
