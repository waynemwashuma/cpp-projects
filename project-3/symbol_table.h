#ifndef __SYMBOL_TABLE__H__
#define __SYMBOL_TABLE__H__

#include <iostream>
#include <unordered_map>
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
};

class SymbolTable
{
private:
  std::unordered_map<std::string, Symbol> inner;

public:
  void add(std::string, Symbol);
  std::pair<Symbol,bool> get(std::string);
  void print();
};

#endif