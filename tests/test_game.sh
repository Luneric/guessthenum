#!/usr/bin/env bash

set -euo pipefail

project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
test_binary="$(mktemp /tmp/guess-the-number-test.XXXXXX)"
trap 'rm -f "$test_binary"' EXIT

g++ -std=c++17 -Wall -Wextra -Wpedantic "$project_root/main.cpp" -o "$test_binary"

output="$(printf 'banana\n' | "$test_binary")"

if [[ "$output" != *"Please enter a valid number."* ]]; then
  echo "Expected non-numeric guesses to be rejected."
  exit 1
fi

game_input="$(seq 1 1000)"
game_input+=$'\ninvalid replay\nY\n'
output="$(printf '%s' "$game_input" | "$test_binary")"

if [[ "$output" != *"You guessed the number!"* ]]; then
  echo "Expected a correct guess to win a round."
  exit 1
fi

if [[ "$output" != *"Please enter valid input."* ]]; then
  echo "Expected an invalid replay response to be rejected."
  exit 1
fi

prompt_count="$(grep -o 'Enter a number from 1 to 1000:' <<<"$output" | wc -l)"
if (( prompt_count < 2 )); then
  echo "Expected uppercase Y to begin a new round."
  exit 1
fi

game_input="$(seq 1 1000)"
game_input+=$'\nn\n'
output="$(printf '%s' "$game_input" | "$test_binary")"

if [[ "$output" != *"You guessed the number!"* ]]; then
  echo "Expected a correct guess before testing lowercase n."
  exit 1
fi

if [[ "$output" != *"Play again? (Y/N): " ]]; then
  echo "Expected lowercase n to quit after the replay prompt."
  exit 1
fi

echo "All game tests passed."
