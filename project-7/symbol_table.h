#ifndef __SYMBOL_TABLE__H__
#define __SYMBOL_TABLE__H__

#include <iostream>
#include <map>
#include "lexer.h"

enum Visibility
{
  VPRIVATE,
  VPUBLIC
};

class Symbol
{
public:
  Visibility visibility;
  std::string type;
  unsigned int token_index;
};

class SymbolTable
{
private:
  std::vector<std::pair<std::string, Symbol>> inner;

public:
  void add(std::string, Symbol);
  Symbol* get(std::string);
  void print(std::vector<Token>);
};

#endif