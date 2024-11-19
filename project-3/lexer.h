#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

enum TokenType
{
  END_OF_FILE = 0,
  IF,
  WHILE,
  DO,
  THEN,
  PRINT,
  PUBLIC,
  PRIVATE,
  PLUS,
  MINUS,
  DIV,
  MULT,
  EQUAL,
  COLON,
  COMMA,
  SEMICOLON,
  LBRAC,
  RBRAC,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  NOTEQUAL,
  GREATER,
  LESS,
  LTEQ,
  GTEQ,
  DOT,
  NUM,
  ID,
  ERROR,
  BASE16NUM, // Add this line
  BASE08NUM, // Add this line
  REALNUM    //,    // Add this line
  // DIGIT16,    // Hexadecimal digits (0-9, A-F)
  // PDIGIT16,   // Hexadecimal digits (1-9, A-F)
  // DIGIT8,     // Octal digits (0-7)
  // PDIGIT8     // Octal digits (1-7)
};

class Token
{
public:
  void Print();

  std::string lexeme;
  TokenType token_type;
  int line_no;
};

class LexicalAnalyzer
{
public:
  Token GetToken();
  TokenType UngetToken(Token);
  LexicalAnalyzer();

private:
  std::vector<Token> tokens;
  int line_no;
  Token tmp;
  InputBuffer input;

  void skip_comment();
  bool skip_space();
  bool IsKeyword(std::string);
  TokenType FindKeywordIndex(std::string);
  Token ScanIdOrKeyword();
  Token ScanNumber();
};

#endif //__LEXER__H__
