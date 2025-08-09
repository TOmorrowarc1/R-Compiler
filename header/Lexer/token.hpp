#pragma once
#include <string>

/*
Define type of tokens:
keywords:
"as","break","const","continue","crate","dyn","else","enum",
"extern","false","fn","for","if","impl","in","let","loop","match","mod",
"move","mut","pub","ref","return","self","Self","static","struct","super",
"trait","true","type","unsafe","use","where","while".
operator&signals:
"LPARENT(","RPARENT)","LBRACK[","RBRACK]","SEMI;","ADD+","MINUS-","MULT*","DIV/","MOD%","&&LOGICAND","||LOGICOR","!LOGICNOT",
"&AND","|OR","^XOR","!NOT","<<LEFTSHIHT","RIGHTSHIFT","GE>=","LE<=","GT>","LT<","EQUAL==","NOTEQUAL!=","ASSIGN".
identifier.
integer.
string.
whitespace.
comment.
*/

enum class TokenType {
  AS,
  BREAK,
  CONST,
  CONTINUE,
  CRATE,
  ELSE,
  ENUM,
  EXTERN,
  FALSE,
  FN,
  FOR,
  IF,
  IMPL,
  IN,
  LET,
  LOOP,
  MATCH,
  MOD,
  MOVE,
  MUT,
  PUB,
  REF,
  RETURN,
  SELF,
  SELF_TYPE,
  STATIC,
  STRUCT,
  SUPER,
  TRAIT,
  TRUE,
  TYPE,
  UNSAFE,
  USE,
  WHERE,
  WHILE,

  ASSIGN,
  EQUAL,
  NOT_EQUAL,
  LESS_THAN,
  LESS_EQUAL,
  GREATER_THAN,
  GREATER_EQUAL,

  LOGIC_AND,
  LOGIC_OR,
  NOT,

  PLUS,
  MINUS,
  MUL,
  DIV,
  MOD_CAL,
  XOR,
  AND,
  OR,
  LEFT_SHIFT,
  RIGHT_SHIFT,

  PLUS_EQUAL,
  MINUS_EQUAL,
  MUL_EQUAL,
  DIV_EQUAL,
  MOD_EQUAL,
  XOR_EQUAL,
  AND_EQUAL,
  OR_EQUAL,
  LEFT_SHIFT_EQUAL,
  RIGHT_SHIFT_EQUAL,

  AT,
  DOT,
  DOT_DOT,
  DOT_DOT_DOT,
  DOT_DOT_EQUAL,

  COMMA,
  SEMICOLON,
  COLON,
  COLON_COLON,

  ARROW,
  LEFT_ARROW,
  FAT_ARROW, //?What?

  HASH, //#
  DOLLAR,
  QUESTION,
  UNDERSCORE, //_

  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  LEFT_PAREN,
  RIGHT_PAREN,

  IDENTIFIER,
  RAWIDENTIFIER,

  CHARLITERAL,

  STRINGLITERAL,
  RAWSTRINGLITERAL,

  BYTELITERAL,
  BYTESTRINGLITERAL,
  RAWBYTESTRINGLITERAL,

  CSTRINGLITERAL,
  RAWCSTRINGLITERAL,

  INTEGERLITERAL,
  FLOATLITERAL,

  WHITESPACE,

  RESERVED
};
struct Token {
  std::string content;
  TokenType type;
};
