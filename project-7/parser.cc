#include <iostream>
#include "parser.h"
#include "symbol_table.h"
#include "lexer.h"

void Parser::parse(std::vector<Token> tokens)
{
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    Token token = tokens[i];

    switch (token.token_type)
    {
    case ID:
      add_variable(token, i);
      break;
    default:
      break;
    }
  }
}

void Parser::infer(std::vector<Token> tokens)
{
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    Token token = tokens[i];

    switch (token.token_type)
    {
    case EQUAL:
      infer_type_assignment(i, tokens);
      break;
    case COLON:
      parse_types(i, tokens);
      break;
        case GREATER:
    case GTEQ:
    case LTEQ:
    case PLUS:
    case MINUS:
    case MULT:
    case DIV:
      infer_operator(i, tokens);
      break;
    default:
      break;
    }
  }
}

void Parser::validate_types(std::vector<Token> tokens)
{
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    auto token = tokens[i];
    std::string type;
    switch (token.token_type)
    {
    case EQUAL:
      validate_assignment(i, tokens);
      break;
    case NOTEQUAL:
    case LESS:
    case GREATER:
    case GTEQ:
    case LTEQ:
    case PLUS:
    case MINUS:
    case MULT:
    case DIV:
      validate_operator(i, tokens);
      i = get_semi(i, tokens);
      break;
    case WHILE:
    case IF:
      if (is_operator(tokens[i + 2]))
      {
        type = std::get<0>(infer_operator(i + 2, tokens));
      }
      else
      {
        type = get_type(tokens[i + 2]);
      }
      if (type != "bool" && type != "")
      {
        std::cout << "TYPE MISMATCH " << token.line_no << " C4" << std::endl;
        exit(0);
      }
      break;
    case SWITCH:
      infer_type_switch(i, tokens);
      if (is_operator(tokens[i + 2]))
      {
        type = std::get<0>(infer_operator(i + 2, tokens));
      }
      else
      {
        type = get_type(tokens[i + 2]);
      }
      if (type != "int" && type != "")
      {
        std::cout << "TYPE MISMATCH " << token.line_no << " C5" << std::endl;
        exit(0);
      }
      break;
    default:
      break;
    }
  }
}

void Parser::validate_assignment(unsigned int index, std::vector<Token> tokens)
{
  Token &lvalue = tokens[index - 1];
  Token &rvalue = tokens[index + 1];

  std::string l_type = get_type(lvalue), r_type;
  if (is_operator(rvalue))
  {
    r_type = std::get<0>(validate_operator(index + 1, tokens));
  }
  else
  {
    r_type = get_type(rvalue);
  }
  if (l_type != r_type)
  {
    std::cout << "TYPE MISMATCH " << rvalue.line_no << " C1" << std::endl;
    exit(0);
  }
}

void Parser::parse_types(unsigned int index, std::vector<Token> tokens)
{
  Token type_token = tokens[index + 1];

  if (tokens[index - 1].token_type != ID)
  {
    return;
  }

  auto type = get_type(type_token);

  for (int i = index - 1; i >= 0; i--)
  {
    auto token = tokens[i];
    if (token.token_type == SEMICOLON || token.token_type == RBRACE)
      break;
    if (token.token_type == ID)
    {
      auto var = get_variable(token.lexeme);
      ;
      if (var != nullptr)
        var->type = type;
    }
  }
}

std::string Parser::infer_type_op(unsigned int index, std::vector<Token> tokens)
{
  std::vector<Token> vars;
  std::string type;
  for (unsigned int i = index; i < tokens.size(); i++)
  {
    auto token = tokens[i];
    if (token.token_type == SEMICOLON || token.token_type == RPAREN)
      break;
    auto the_type = get_type(token);
    if (the_type != "")
      type = the_type;
    if (token.token_type == ID)
    {
      vars.push_back(token);
    }
  }
  for (auto token : vars)
  {
    auto var_sym = get_variable(token.lexeme);
    if (var_sym == nullptr || var_sym->type != "")
      continue;
    var_sym->type = type;
  }
  switch (tokens[index].token_type)
  {
  case NOT:
  case NOTEQUAL:
  case LESS:
  case GREATER:
  case GTEQ:
  case LTEQ:
    return "bool";
  default:
    return type;
  }
}

std::tuple<std::string, int> Parser::infer_operator(unsigned int op, std::vector<Token> tokens)
{
  Token &current = tokens[op];
  Token &lvalue = tokens[op + 1];
  auto l_type = get_type(lvalue);
  int r_idx = 2;
  if (is_operator(lvalue))
  {
    auto oper = infer_operator(op + 1, tokens);
    l_type = std::get<0>(oper);
    r_idx += std::get<1>(oper);
  }

  Token &rvalue = tokens[op + r_idx];
  auto r_type = get_type(rvalue);

  if (is_operator(rvalue))
  {
    r_type = std::get<0>(infer_operator(op + r_idx, tokens));
  }

  if (l_type == "" && lvalue.token_type == ID && r_type != "")
  {
    auto var = get_variable(lvalue.lexeme);
    if (var != nullptr)
      var->type = r_type;
  }

  if (r_type == "" && rvalue.token_type == ID && l_type != "")
  {
    auto var = get_variable(rvalue.lexeme);
    if (var != nullptr)
      var->type = l_type;
    std::cout << "";
  }

  auto type = l_type == "" ? r_type : l_type;
  switch (current.token_type)
  {
  case NOT:
    return std::make_tuple("bool", r_idx - 1);
  case NOTEQUAL:
  case LESS:
  case GREATER:
  case GTEQ:
  case LTEQ:
    return std::make_tuple("bool", r_idx);
  case PLUS:
  case MINUS:
  case MULT:
  case DIV:
    return std::make_tuple(type, r_idx);
  default:
    return std::make_tuple("", 0);
  }
}

void Parser::infer_type_switch(unsigned int index, std::vector<Token> tokens)
{
  std::vector<Token> vars;
  std::string type;
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    auto token = tokens[i];
    if (token.token_type == RPAREN)
      break;
    auto the_type = get_type(token);
    if (the_type != "")
      type = the_type;
    if (token.token_type == ID)
    {
      vars.push_back(token);
    }
  }

  if (type == "")
  {
    type = "int";
  }
  for (auto token : vars)
  {
    auto var_sym = get_variable(token.lexeme);
    if (var_sym == nullptr)
      continue;
    var_sym->type = type;
  }
}

void Parser::infer_type_assignment(unsigned int index, std::vector<Token> tokens)
{
  auto lvalue = tokens[index - 1];
  auto rvalue = tokens[index + 1];

  std::string r_type, l_type = get_type(lvalue);
  if (is_operator(rvalue))
  {
    r_type = std::get<0>(infer_operator(index + 1, tokens));
  }
  else
  {
    r_type = get_type(rvalue);
  }

  if (l_type == "")
  {
    auto sym = get_variable(lvalue.lexeme);
    if (sym != nullptr)
    {
      sym->type = r_type;
    }
  }
}

std::string Parser::get_type(Token token)
{
  auto type = token.token_type;
  switch (type)
  {
  case INT:
  case NUM:
    return "int";
  case REAL:
  case REALNUM:
    return "real";
  case BOOL:
  case TRUE:
  case FALSE:
    return "bool";
  case ID:
  {
    auto var = get_variable(token.lexeme);
    if (var != nullptr)
    {
      return var->type;
    }
    return "";
  }
  default:
    return "";
  }
  return "";
}

void Parser::add_variable(Token token, unsigned int index)
{
  std::string hash = token.lexeme;

  Symbol symbol;
  symbol.visibility = current_visibility;
  symbol.token_index = index;
  symbol_table.add(hash, symbol);
}

Symbol *Parser::get_variable(std::string name)
{
  auto symbol = symbol_table.get(name);
  return symbol;
}

std::tuple<std::string, int> Parser::validate_operator(unsigned int op, std::vector<Token> tokens)
{
  Token &current = tokens[op];
  Token &lvalue = tokens[op + 1];
  auto l_type = get_type(lvalue);
  int r_idx = 2;
  if (is_operator(lvalue))
  {
    auto oper = validate_operator(op + 1, tokens);
    l_type = std::get<0>(oper);
    r_idx += std::get<1>(oper);
  }

  Token &rvalue = tokens[op + r_idx];
  auto r_type = get_type(rvalue);

  if (is_operator(rvalue))
  {
    r_type = std::get<0>(validate_operator(op + r_idx, tokens));
  }
  switch (current.token_type)
  {
  case NOT:
    if (l_type != "bool")
    {
      std::cout << "TYPE MISMATCH " << current.line_no << " C3" << std::endl;
      exit(1);
    }
    return std::make_tuple("bool", r_idx - 1);
  case NOTEQUAL:
  case LESS:
  case GREATER:
  case GTEQ:
  case LTEQ:
    if (l_type != r_type)
    {
      std::cout << "TYPE MISMATCH " << current.line_no << " C2" << std::endl;
      exit(1);
    }
    return std::make_tuple("bool", r_idx);
  case PLUS:
  case MINUS:
  case MULT:
  case DIV:
    if (l_type != r_type)
    {
      std::cout << "TYPE MISMATCH " << current.line_no << " C2" << std::endl;
      exit(1);
    }
    return std::make_tuple(l_type, r_idx);
  default:
    break;
  }
  return std::make_tuple("", 0);
}

void Parser::print_types(std::vector<Token> tokens)
{
  std::map<std::string, Symbol> symbols;
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    if (is_operator(tokens[i]))
    {
      print_type_op(i, tokens, symbols);
      i = get_semi(i, tokens);
    }
    else if (tokens[i].token_type == ID)
    {
      auto name = tokens[i].lexeme;
      auto sym = get_variable(name);
      if (sym == nullptr)
        continue;

      symbols[name] = (*sym);
    }
  }

  std::string prev_type = symbols.begin()->second.type;

  for (auto it = symbols.begin(); it != symbols.end(); it++)
  {
    if (it->second.type != "")
    {
      if (prev_type == "")
        std::cout << ": ? #" << std::endl;
      std::cout << it->first << ": " << it->second.type << " #" << std::endl;
      prev_type = it->second.type;
      continue;
    }
    if (prev_type == "" && it != symbols.begin())
      std::cout << ", ";
    std::cout << it->first;
    if (symbols.end() == it)
      prev_type = it->second.type;
  }
  if (prev_type == "")
  {
    std::cout << ": ? #" << std::endl;
  }
  // std::cout << std::endl;
}

void Parser::print_type_op(unsigned int op, std::vector<Token> tokens, std::map<std::string, Symbol> &results)
{
  Token &lvalue = tokens[op + 1];
  Token &rvalue = tokens[op + 2];
  std::string l_type = get_type(lvalue),
              r_type = get_type(rvalue);
  if (is_operator(lvalue))
  {
    print_type_op(op + 1, tokens, results);
  }
  else if (lvalue.token_type == ID)
  {
    auto sym = get_variable(lvalue.lexeme);
    if (sym == nullptr)
      return;
    results.insert(std::make_pair(lvalue.lexeme, *sym));
  }
  if (is_operator(rvalue))
  {
    print_type_op(op + 2, tokens, results);
  }
  else if (rvalue.token_type == ID)
  {
    auto sym = get_variable(rvalue.lexeme);
    if (sym == nullptr)
      return;
    results.insert(std::make_pair(lvalue.lexeme, *sym));
  }
}

int get_semi(unsigned int index, std::vector<Token> tokens)
{
  for (unsigned int i = index; i < tokens.size(); i++)
  {
    if (tokens[i].token_type == SEMICOLON)
      return i;
  }
  return -1;
}

bool is_operator(Token &token)
{
  switch (token.token_type)
  {
  case NOTEQUAL:
  case LESS:
  case GREATER:
  case GTEQ:
  case LTEQ:
  case PLUS:
  case MINUS:
  case MULT:
  case DIV:
  case NOT:
    return true;
  default:
    return false;
  }
  return false;
}
