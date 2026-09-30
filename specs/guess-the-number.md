# Spec: Guess the Number Game

## Assumptions

1. The intended product is a terminal-based C++ game, rather than a graphical or web application.
2. Each round uses a newly generated, uniformly distributed secret integer in the inclusive range 1 through 1000.
3. `Y` and `y` start another round; `N` and `n` end the program. Whitespace around a response is ignored.
4. The existing `main.cpp` implementation is the behavioral reference for this specification.

## Objective

Provide a small command-line guessing game for a person running the program locally. In each round, the program selects a secret number from 1 to 1000 and asks the player to guess it. After each valid, incorrect guess, it tells the player whether to guess higher or lower. Once the player succeeds, it confirms the win and offers another round.

### Acceptance criteria

- The program prompts for a number from 1 through 1000.
- A valid guess below the secret produces `Guess higher.`; a valid guess above it produces `Guess lower.`
- An exact guess produces `You guessed the number!` and then asks `Play again? (Y/N): `.
- `Y`/`y` begins a fresh round with a newly generated secret number; `N`/`n` exits normally.
- Non-numeric, partially numeric, blank, and out-of-range guesses are rejected with `Please enter a valid number.` without ending the round.
- Any replay response other than `Y`, `y`, `N`, or `n` (with optional surrounding whitespace) is rejected with `Please enter valid input.` and is requested again.
- End-of-input exits cleanly without an error.

## Tech Stack

- C++17
- C++ standard library (`<charconv>`, `<cctype>`, `<iostream>`, `<random>`, `<string>`, and `<string_view>`)
- Bash test scripts
- `g++` compiler

## Commands

Run the game:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o app
./app
```

Run the automated tests:

```bash
bash test_runner.sh
```

## Project Structure

```text
main.cpp            # Game loop, input parsing, feedback, and replay handling
test_runner.sh      # Entrypoint for all automated tests
tests/test_game.sh  # Black-box terminal behavior tests
specs/              # Feature specifications
```

## Code Style

- Use C++17 and compile with `-Wall -Wextra -Wpedantic`.
- Use `camelCase` for functions and local variables; use `k`-prefixed `PascalCase` for constants.
- Keep input validation in small named functions and keep `main` focused on the game flow.
- Parse integer input strictly: after trimming whitespace, every character must belong to one in-range integer.
- Print one clear line of feedback for each invalid or incorrect action.

```cpp
if (guess < secretNumber) {
  std::cout << "Guess higher.\n";
} else if (guess > secretNumber) {
  std::cout << "Guess lower.\n";
} else {
  std::cout << "You guessed the number!\n";
  break;
}
```

## Testing Strategy

Tests are Bash-based black-box tests in `tests/test_game.sh`. They compile `main.cpp` using the required warning flags and feed scripted input to the executable.

- Test invalid guesses, including non-numeric input.
- Test that an exact guess completes a round.
- Test invalid replay input and both case variants of valid replay choices.
- Test that accepting replay starts another round and declining replay ends the session.
- Run `bash test_runner.sh` before committing changes to game behavior.

## Boundaries

- Always: validate all player input, preserve the inclusive 1–1000 range, run `bash test_runner.sh` before committing, and update this spec when behavior changes.
- Ask first: add dependencies, change the C++ language standard, alter the random-number range, modify test infrastructure, or add persistent scores/accounts.
- Never: commit built binaries or secrets, remove a failing test merely to make the suite pass, or accept malformed numeric input as a guess.

## Success Criteria

- `g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o app` succeeds without warnings.
- `bash test_runner.sh` exits successfully.
- A manual play session demonstrates the prompt, higher/lower guidance, win message, and replay choice.
- The behavior satisfies every acceptance criterion in this document.

## Open Questions

None for the current terminal-game scope. Future changes such as difficulty levels, attempt counts, score tracking, or a graphical interface require a spec update and review before implementation.
