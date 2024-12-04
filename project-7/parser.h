#ifndef __PARSER_H__
#define __PARSER__H__

#include <iostream>
#include <map>
#include "symbol_table.h"
#include "lexer.h"

class Parser
{
public:
  SymbolTable symbol_table;
  Visibility current_visibility;
  void parse(std::vector<Token>);
  void infer(std::vector<Token> tokens);
  void validate_types(std::vector<Token> tokens);
  void parse_types(unsigned int, std::vector<Token>);
  std::string infer_type_op(unsigned int index, std::vector<Token> tokens);
  void infer_type_switch(unsigned int index, std::vector<Token> tokens);
  void infer_type_assignment(unsigned int index, std::vector<Token> tokens);
  void add_variable(Token, unsigned int);
  Symbol *get_variable(std::string);
  std::string get_type(Token);
  std::tuple<std::string, int> infer_operator(unsigned int, std::vector<Token>);
  std::tuple<std::string, int> validate_operator(unsigned int, std::vector<Token>);
  void validate_assignment(unsigned int, std::vector<Token>);
  void print_types(std::vector<Token>);
  void print_type_op(unsigned int op, std::vector<Token>, std::map<std::string, Symbol> &);
};

int get_semi(unsigned int index, std::vector<Token> tokens);
bool is_operator(Token &token);
#endif