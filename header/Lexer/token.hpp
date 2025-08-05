#pragma once

#include <string>

/*
Define type of tokens:
keywords-"i32","bool","str","true","false","let","fn","if","else","for","while","break","continue","return".
operator&signals-"LPARENT(","RPARENT)","LBRACK[","RBRACK]","SEMI;","ADD+","MINUS-","MULT*","DIV/","MOD%","&&LOGICAND","||LOGICOR","LOGICNOT",
"&AND","|OR","^XOR","!NOT","<<LEFTSHIHT","RIGHTSHIFT","GE>=","LE<=","GT>","LT<","EQUAL==","NOTEQUAL!=","ASSIGN".
identifier.
integer.
string.
whitespace.
comment.
*/

enum class TokenType {
  I32,
  BOOL,
  STR,
  TRUE,
  FALSE,
  LET,
  FN,
  IF,
  ELSE,
  FOR,
  WHILE,
  BREAK,
  CONTINUE,
  RETURN,
  LPARENT,
  RPARENT,
  LBRACK,
  RBRACK,
  SEMI,
  ADD,
  MINUS,
  MUL,
  DIV,
  MOD,
  LOGICAND,
  LOGICOR,
  LOGICNOT,
  AND,
  OR,
  XOR,
  NOT,
  LEFTSHIFT,
  RIGHTSHIFT,
  GE,
  LE,
  GT,
  LT,
  EQUAL,
  NOTEQUAL,
  ASSIGN,
  IDENTIFIER,
  INTEGER,
  STRING,
  WHITESPACE,
  COMMENT,
};

struct Token {
  std::string content;
  TokenType type;
};