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
    parser.init();
    parser.parse(tokens);

    parser.print_assignments(tokens);
}
