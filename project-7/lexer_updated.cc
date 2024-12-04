#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"

using namespace std;

// Token labels
string reserved[] = {
    "END_OF_FILE",
    "IF",
    "WHILE",
    "DO",
    "SWITCH",
    "CASE",
    "THEN",
    "PRINT",
    "PUBLIC",
    "PRIVATE",
    "INT",
    "REAL",
    "BOOL",
    "TRUE",
    "FALSE",
    "PLUS",
    "MINUS",
    "DIV",
    "MULT",
    "EQUAL",
    "COLON",
    "COMMA",
    "SEMICOLON",
    "LBRAC",
    "RBRAC",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "NOTEQUAL",
    "GREATER",
    "LESS",
    "LTEQ",
    "GTEQ",
    "NOT",
    "DOT",
    "NUM",
    "ID",
    "ERROR",
    //"DIGIT16",
    //"PDIGIT16",
    //"DIGIT8",
    // "PDIGIT8" , // New token labels
    "BASE16NUM", // Add this line
    "BASE08NUM", // Add this line
    "REALNUM"};

#define KEYWORDS_COUNT 14
string keyword[] = {"if", "while", "do", "then", "switch", "case", "print", "public", "private", "int", "real", "bool", "true", "false"};

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int)this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

void LexicalAnalyzer::skip_comment()
{
    char c;
    std::string sentence;

    input.GetChar(c);
    sentence.push_back(c);
    if (input.EndOfInput() || c != '/')
    {
        input.UngetChar(c);
        return;
    }

    input.GetChar(c);
    if (input.EndOfInput() || c != '/')
    {
        input.UngetString(sentence);
        return;
    }

    while (!input.EndOfInput() && c != '\n')
    {
        line_no += (c == '\n');
        sentence += c;
        input.GetChar(c);
    }
    input.UngetChar('\n');
}
bool LexicalAnalyzer::skip_space()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c))
    {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput())
    {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
    {
        if (s == keyword[i])
        {
            return (TokenType)(i + 1);
        }
    }
    return ERROR;
}
Token LexicalAnalyzer::ScanNumber()
{
    char currentChar;
    char nextChar1;
    char nextChar2;
    char nextChar3;
    char nextChar4;
    char nextChar5;
    char nextChar6;
    bool containsEight = false;

    input.GetChar(currentChar);
    if (isdigit(currentChar))
    {
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        if (currentChar == '0')
        {
            tmp.lexeme = "0";
        }
        else
        {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(currentChar))
            {
                tmp.lexeme += currentChar;
                if (currentChar == '8')
                {
                    containsEight = true;
                }
                input.GetChar(currentChar);
            }
            if (!input.EndOfInput())
            {
                input.UngetChar(currentChar);
            }
        }

        // TODO: You can check for REALNUM, BASE08NUM, and BASE16NUM here!
        input.GetChar(nextChar1);
        switch (nextChar1)
        {
        case 'x':
            input.GetChar(nextChar2);
            if (nextChar2 == '0')
            {
                input.GetChar(nextChar3);
                if (nextChar3 == '8')
                {
                    if (containsEight)
                    {
                        containsEight = false;
                        input.UngetChar(nextChar3);
                        input.UngetChar(nextChar2);
                        input.UngetChar(nextChar1);
                        tmp.token_type = NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                    else
                    {
                        tmp.lexeme += "x08";
                        tmp.token_type = BASE08NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }
                else
                {
                    input.UngetChar(nextChar3);
                    input.UngetChar(nextChar2);
                    input.UngetChar(nextChar1);
                    tmp.token_type = NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
            }
            else if (nextChar2 == '1')
            {
                input.GetChar(nextChar3);
                if (nextChar3 == '6')
                {
                    tmp.lexeme += "x16";
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
                else
                {
                    input.UngetChar(nextChar3);
                    input.UngetChar(nextChar2);
                    input.UngetChar(nextChar1);
                }
            }
            else
            {
                input.UngetChar(nextChar1);
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
        case 'A':
            tmp.lexeme += nextChar1;
            input.GetChar(nextChar2);
            if (nextChar2 == 'B')
            {
                tmp.lexeme += nextChar2;
                input.GetChar(nextChar3);
                if (nextChar3 == '1')
                {
                    tmp.lexeme += nextChar3;
                    input.GetChar(nextChar4);
                    if (nextChar4 == 'x')
                    {
                        input.GetChar(nextChar5);
                        if (nextChar5 == '1')
                        {
                            input.GetChar(nextChar6);
                            if (nextChar6 == '6')
                            {
                                tmp.lexeme += "x16";
                                tmp.token_type = BASE16NUM;
                                tmp.line_no = line_no;
                                return tmp;
                            }
                            else
                            {
                                input.UngetChar(nextChar6);
                                input.UngetChar(nextChar5);
                                input.UngetChar(nextChar4);
                                input.UngetChar(nextChar3);
                                input.UngetChar(nextChar2);
                                input.UngetChar(nextChar1);
                            }
                        }
                    }
                }
            }
            else
            {
                input.UngetChar(nextChar1);
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }

        case '.':
            input.GetChar(currentChar);
            if (isdigit(currentChar))
            {
                tmp.lexeme += '.';
                while (!input.EndOfInput() && isdigit(currentChar))
                {
                    tmp.lexeme += currentChar;
                    input.GetChar(currentChar);
                    tmp.token_type = REALNUM;
                    tmp.line_no = line_no;
                }
                if (currentChar == '0')
                {
                    tmp.lexeme += '0';
                }
                input.UngetChar(currentChar);
                return tmp;
            }
            else
            {
                input.UngetChar(currentChar);
                input.UngetChar(nextChar1);
                tmp.line_no = line_no;
                return tmp;
            }

        default:
            input.UngetChar(nextChar1);
            tmp.token_type = NUM;
            tmp.line_no = line_no;
            return tmp;
        }
        // TODO: Finish check
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(currentChar);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c))
    {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c))
        {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);
    return tok.token_type;
}

// Updated GetToken function to call ScanNumber for numeric input
Token LexicalAnalyzer::GetToken()
{
    char c;

    if (!tokens.empty())
    {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    skip_space();
    skip_comment();
    tmp.lexeme = "";
    tmp.line_no = line_no;

    input.GetChar(c);

    switch (c)
    {
    case '.':
        tmp.token_type = DOT;
        return tmp;
    case '+':
        tmp.token_type = PLUS;
        return tmp;
    case '-':
        tmp.token_type = MINUS;
        return tmp;
    case '/':
        tmp.token_type = DIV;
        return tmp;
    case '*':
        tmp.token_type = MULT;
        return tmp;
    case '=':
        tmp.token_type = EQUAL;
        return tmp;
    case ':':
        tmp.token_type = COLON;
        return tmp;
    case ',':
        tmp.token_type = COMMA;
        return tmp;
    case ';':
        tmp.token_type = SEMICOLON;
        return tmp;
    case '[':
        tmp.token_type = LBRAC;
        return tmp;
    case ']':
        tmp.token_type = RBRAC;
        return tmp;
    case '(':
        tmp.token_type = LPAREN;
        return tmp;
    case ')':
        tmp.token_type = RPAREN;
        return tmp;
    case '{':
        tmp.token_type = LBRACE;
        return tmp;
    case '}':
        tmp.token_type = RBRACE;
        return tmp;
    case '!':
        input.GetChar(c);
        if (c == '=')
        {
            tmp.token_type = NOTEQUAL;
        }
        else
        {
            input.UngetChar(c);
            tmp.token_type = NOT;
        }
        return tmp;
    case '<':
        input.GetChar(c);
        if (c == '=')
        {
            tmp.token_type = LTEQ;
        }
        else
        {
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = LESS;
        }
        return tmp;
    case '>':
        input.GetChar(c);
        if (c == '=')
        {
            tmp.token_type = GTEQ;
        }
        else
        {
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = GREATER;
        }
        return tmp;
    default:
        if (isdigit(c))
        {
            input.UngetChar(c);
            return ScanNumber(); // Use ScanNumber for digit handling
        }
        else if (isalpha(c))
        {
            input.UngetChar(c);
            return ScanIdOrKeyword();
        }
        else if (input.EndOfInput())
        {
            tmp.token_type = END_OF_FILE;
        }
        else
        {
            tmp.token_type = ERROR;
        }
        return tmp;
    }

}