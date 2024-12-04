#include "lexer.h"
#include "parser.h"

int main()
{
    LexicalAnalyzer lexer;
    std::vector<Token> tokens;
    Token token = lexer.GetToken();
    while (token.token_type != END_OF_FILE)
    {
        tokens.push_back(token);
        token = lexer.GetToken();
    }

    Parser parser;
    parser.parse(tokens);
    parser.infer(tokens);
    parser.infer(tokens);
    parser.infer(tokens);
    parser.infer(tokens);
    parser.validate_types(tokens);
    parser.symbol_table.print(tokens);
}
