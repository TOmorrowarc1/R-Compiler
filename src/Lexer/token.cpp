#include "token.hpp"
#include <stdexcept>

TokenStream::TokenStream(const std::vector<Token> &tokens)
    : tokens(tokens), index(0) {}

auto TokenStream::next() -> Token {
  if (index < tokens.size()) {
    return tokens[index++];
  }
  return Token{"", TokenType::END_OF_FILE};
}

auto TokenStream::peek() const -> Token {
  if (index < tokens.size()) {
    return tokens[index];
  }
  return Token{"", TokenType::END_OF_FILE};
}

auto TokenStream::getIndex() const -> int32_t { return index; }

void TokenStream::restoreIndex(int32_t newIndex) {
  if (newIndex >= 0 && newIndex < tokens.size()) {
    index = newIndex;
  } else {
    throw std::out_of_range("Index out of range in TokenStream.");
  }
}
