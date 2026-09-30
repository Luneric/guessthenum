#include <charconv>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <string_view>

namespace {

constexpr int kMinimumNumber = 1;
constexpr int kMaximumNumber = 1000;

enum class ReplayReadResult { kInputEnded, kInvalid, kValid };

std::string_view trimWhitespace(std::string_view text) {
  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
    text.remove_prefix(1);
  }

  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
    text.remove_suffix(1);
  }

  return text;
}

bool parseGuess(const std::string& input, int& guess) {
  const std::string_view trimmedInput = trimWhitespace(input);
  if (trimmedInput.empty()) {
    return false;
  }

  const char* first = trimmedInput.data();
  const char* last = first + trimmedInput.size();
  const auto [end, error] = std::from_chars(first, last, guess);

  return error == std::errc{} && end == last && guess >= kMinimumNumber &&
         guess <= kMaximumNumber;
}

ReplayReadResult readReplayChoice(bool& playAgain) {
  std::string input;
  if (!std::getline(std::cin, input)) {
    return ReplayReadResult::kInputEnded;
  }

  const std::string_view choice = trimWhitespace(input);
  if (choice.size() == 1) {
    const char normalizedChoice =
        static_cast<char>(std::tolower(static_cast<unsigned char>(choice.front())));
    if (normalizedChoice == 'y') {
      playAgain = true;
      return ReplayReadResult::kValid;
    }
    if (normalizedChoice == 'n') {
      playAgain = false;
      return ReplayReadResult::kValid;
    }
  }

  std::cout << "Please enter valid input.\n";
  return ReplayReadResult::kInvalid;
}

}  // namespace

int main() {
  std::random_device seedSource;
  std::mt19937 randomEngine(seedSource());
  std::uniform_int_distribution<int> numberDistribution(kMinimumNumber, kMaximumNumber);

  bool playAgain = true;
  while (playAgain) {
    const int secretNumber = numberDistribution(randomEngine);

    while (true) {
      std::cout << "Enter a number from 1 to 1000: ";

      std::string input;
      if (!std::getline(std::cin, input)) {
        return 0;
      }

      int guess = 0;
      if (!parseGuess(input, guess)) {
        std::cout << "Please enter a valid number.\n";
        continue;
      }

      if (guess < secretNumber) {
        std::cout << "Guess higher.\n";
      } else if (guess > secretNumber) {
        std::cout << "Guess lower.\n";
      } else {
        std::cout << "You guessed the number!\n";
        break;
      }
    }

    while (true) {
      std::cout << "Play again? (Y/N): ";
      const ReplayReadResult result = readReplayChoice(playAgain);
      if (result == ReplayReadResult::kInputEnded) {
        return 0;
      }
      if (result == ReplayReadResult::kValid) {
        break;
      }
    }
  }

  return 0;
}
