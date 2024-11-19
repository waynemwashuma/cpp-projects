#ifndef __PARSER_H__
#define __PARSER__H__

#include <iostream>
#include "symbol_table.h"
#include "lexer.h"

class Parser
{
public:
  void init();
  SymbolTable symbol_table;
  std::vector<std::string> scope_stack;
  std::string current_scope;
  Visibility current_visibility;
  void syntax_error();
  void parse(std::vector<Token>);
  void parse_variable(unsigned int, std::vector<Token>);
  void print_assignments(std::vector<Token>);
  void add_scope(std::string);
  void remove_scope();
  void add_variable(Token);
  std::tuple<std::string,Symbol,bool> get_variable(std::string);
  std::string hash_symbol(std::string);
};

#endif