#!/usr/bin/env bash

set -e

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <day> <part> <answer>" >&2
    exit 1
fi

DAY=$1
PART=$2
ANSWER=$3

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

if [ "$DAY" -lt 1 ] || [ "$DAY" -gt 25 ]; then
    echo "Error: Invalid day $DAY (must be 1-25)" >&2
    exit 1
fi

if [ "$PART" != "1" ] && [ "$PART" != "2" ]; then
    echo "Error: Invalid part $PART (must be 1 or 2)" >&2
    exit 1
fi

echo "Submitting answer: $ANSWER for day $DAY part $PART..."

RESPONSE=$(curl -s -X POST \
    -H "Cookie: session=$AOC_SESSION" \
    -d "level=$PART&answer=$ANSWER" \
    "https://adventofcode.com/${AOC_YEAR}/day/${DAY}/answer")

# Check for various response patterns
if echo "$RESPONSE" | grep -q "You gave an answer too recently"; then
    echo -e "\033[41mYou gave an answer too recently. Please wait.\033[m"
    exit 1
elif echo "$RESPONSE" | grep -q "That's not the right answer"; then
    ERROR_MSG=$(echo "$RESPONSE" | grep -o "That's not the right answer[^.]*\." | head -1)
    echo -e "\033[41m${ERROR_MSG}\033[m"
    exit 1
elif echo "$RESPONSE" | grep -q "You don't seem to be solving"; then
    ERROR_MSG=$(echo "$RESPONSE" | grep -o "You don't seem to be solving[^?]*?" | head -1)
    echo -e "\033[41m${ERROR_MSG}\033[m"
    exit 1
elif echo "$RESPONSE" | grep -q "That's the right answer"; then
    echo -e "\033[42mThat's the right answer!\033[m"
    exit 0
else
    # Unexpected response - show it for debugging
    echo "Unexpected response:"
    echo "$RESPONSE"
    exit 1
fi
