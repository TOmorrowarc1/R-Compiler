#pragma once
#include "Token.hpp"

/*
The Lexer converts the string, a plain copy of the whole program into a stream
of tokens, whose type is defined and exposed in Token.hpp, and changes raw
strings into plain strings.
*/

auto lex(const std::string &target) -> std::vector<Token>;