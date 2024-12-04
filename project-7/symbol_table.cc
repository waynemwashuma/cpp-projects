#include <iostream>
#include <unordered_map>
#include "lexer.h"
#include "symbol_table.h"

void SymbolTable::add(std::string name, Symbol symbol)
{
  for (unsigned int i = 0; i < inner.size(); i++)
  {
    if (inner[i].first == name)
      return;
  }

  inner.push_back(std::make_pair(name, symbol));
}

Symbol *SymbolTable::get(std::string name)
{

  for (unsigned int i = 0; i < inner.size(); i++)
  {
    if (inner[i].first == name)
      return &inner.at(i).second;
  }
  return nullptr;
}

void SymbolTable::print(std::vector<Token> tokens)
{
  std::string prev_type = inner.begin()->second.type;
  if (prev_type == "")
  {
    std::cout << inner.begin()->first;
  }
  else
  {
    if (prev_type == "")
      std::cout << ": ? #" << std::endl;
    std::cout << inner.begin()->first << ": " << inner.begin()->second.type << " #" << std::endl;
  }
  for (auto pair = inner.begin() + 1; pair != inner.end(); pair++)
  {
    if (prev_type == "" && pair->second.type == "")
    {
      std::cout << ", ";
    }
    if (pair->second.type == "")
    {
      std::cout << pair->first;
    }
    else
    {
      if (prev_type == "")
        std::cout << ": ? #" << std::endl;
      std::cout << pair->first << ": " << pair->second.type << " #" << std::endl;
    }
    prev_type = pair->second.type;
  }
  if (prev_type == "")
    std::cout << ": ? #" << std::endl;
}