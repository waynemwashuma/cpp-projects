#include <iostream>
#include "parser.h"
#include "symbol_table.h"
#include "lexer.h"
#define GLOBAL_SCOPE_MARKER "global"

void Parser::init()
{
  current_scope = GLOBAL_SCOPE_MARKER;
  current_visibility = VPUBLIC;
}

void Parser::syntax_error()
{
  std::cout << "Syntax Error\n";
  exit(1);
}

void Parser::parse(std::vector<Token> tokens)
{
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    Token token = tokens[i];

    switch (token.token_type)
    {
    case ID:
      parse_variable(i, tokens);
      break;
    case RBRACE:
      remove_scope();
      current_visibility = VPUBLIC;
      break;
    case PRIVATE:
      current_visibility = VPRIVATE;
      break;
    case PUBLIC:
      current_visibility = VPUBLIC;
      break;
    default:
      break;
    }
  }
}

void Parser::parse_variable(unsigned int index, std::vector<Token> tokens)
{
  Token token = tokens[index];
  Token next_token = tokens[index + 1];
  switch (next_token.token_type)
  {
  case SEMICOLON:
  case COMMA:
    if (index > 0)
    {
      Token previous_token = tokens[index - 1];
      switch (previous_token.token_type)
      {
      case COMMA:
      case COLON:
      case SEMICOLON:
      case DOT:
      case LBRACE:
      case RBRACE:
        add_variable(token);
        break;
      default:
        break;
      }
    }
    else
    {
      add_variable(token);
    }
    break;
  case LBRACE:
    add_scope(token.lexeme);
    break;
  case EQUAL:
  case LESS:
  case GREATER:
    break;
  default:
    syntax_error();
    break;
  }
}

void Parser::remove_scope()
{
  scope_stack.pop_back();
  if (scope_stack.size() == 0)
  {
    current_scope = GLOBAL_SCOPE_MARKER;
  }
  else
  {
    current_scope = scope_stack.back();
  }
}

void Parser::add_scope(std::string scope)
{
  current_scope = scope;
  scope_stack.push_back(scope);
}

std::string Parser::hash_symbol(std::string name)
{
  if (current_scope == GLOBAL_SCOPE_MARKER)
  {
    return "::" + name;
  }
  return current_scope + "." + name;
}

void Parser::add_variable(Token token)
{
  std::string hash = hash_symbol(token.lexeme);

  Symbol symbol;
  symbol.visibility = current_visibility;
  symbol_table.add(hash, symbol);
}

std::tuple<std::string, Symbol, bool> Parser::get_variable(std::string name)
{
  for (int i = scope_stack.size() - 1; i >= 0; i--)
  {
    std::string scope = scope_stack[i];
    std::string hash = scope + "." + name;
    std::pair<Symbol, bool> symbol = symbol_table.get(hash);

    if (!symbol.second || (symbol.first.visibility == VPRIVATE && current_scope != scope))
    {
      continue;
    }
    return std::tuple<std::string, Symbol, bool>(hash, symbol.first, symbol.second);
  }

  std::string hash = "::" + name;
  std::pair<Symbol, bool> symbol = symbol_table.get(hash);
  if (!symbol.second)
  {
    hash = "?." + name;
  }
  return std::tuple<std::string, Symbol, bool>(hash, symbol.first, symbol.second);
}

void Parser::print_assignments(std::vector<Token> tokens)
{
  for (unsigned int i = 1; i < tokens.size() - 1; i++)
  {
    Token previous_token = tokens[i - 1];
    Token token = tokens[i];
    Token next_token = tokens[i + 1];

    if (token.token_type == LBRACE)
    {
      add_scope(previous_token.lexeme);
    }
    if (token.token_type == RBRACE)
    {
      remove_scope();
    }
    if (token.token_type == EQUAL)
    {
      auto lvalue_var = get_variable(previous_token.lexeme);
      auto rvalue_var = get_variable(next_token.lexeme);

      auto lvalue_name = std::get<0>(lvalue_var);
      auto rvalue_name = std::get<0>(rvalue_var);

      std::cout << lvalue_name << " = " << rvalue_name << "\n";
    }
  }
}
