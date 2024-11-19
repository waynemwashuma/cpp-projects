#include <iostream>
#include <unordered_map>
#include "lexer.h"
#include "symbol_table.h"

void SymbolTable::add(std::string name, Symbol symbol)
{
  inner.insert(std::pair<std::string, Symbol>(name, symbol));
}

std::pair<Symbol,bool> SymbolTable::get(std::string name)
{
  std::pair<Symbol,bool> result;
  Symbol inter;
  try
  {
    inter = inner.at(name);
    result = std::make_pair(inter,true);
  }
  catch (const std::exception &e)
  {
    result = std::make_pair(inter,false);
  }
  return result;
}

void SymbolTable::print()
{
  std::cout << "{" << std::endl;
  for (auto pair : inner)
  {
    std::cout << "  { " << pair.first << ", " << pair.second.visibility << " }" << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "size: " << inner.size() << std::endl;
}