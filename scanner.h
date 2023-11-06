//
// Created by Faruq Yusuff on 10/18/23.
//

#ifndef LOX_INTERPRETER_SCANNER_H
#define LOX_INTERPRETER_SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

using namespace std; // todo: remove - bad practice

class Scanner {
    public:
        Scanner (string source);

        vector<Token> scanTokens();

    private:
        string source;
        vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

        static const unordered_map<string, TokenType> keywords;

        bool isAtEnd();

        char advance();

        void addToken(TokenType type);

        void addToken(TokenType type, string literal);

        bool isAlpha(char c);

        bool isAlphaNumeric(char c);

        bool match(char expected);

        char peek();

        char peekNext();

        void stringFunc();

        void number();

        void identifier();

        void scanToken();
};



#endif //LOX_INTERPRETER_SCANNER_H