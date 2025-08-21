#pragma once
#include "Token.hpp"

/*
In the Lexer, I convert the input string into a token stream and do nothing
else except make raw literals into common ones.
*/

auto lex(const std::string &target) -> std::vector<Token>;