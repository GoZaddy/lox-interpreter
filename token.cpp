#include <sstream>

#include "token.h"

Token::Token(TokenType type, string lexeme, string literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
};

string Token::toString(){
    stringstream ss;

    ss << this->type << " " << this->lexeme << " " << this->literal << endl; 
    return ss.str();
};
