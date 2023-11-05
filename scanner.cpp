#include "scanner.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "util.h"
#include "token.h"

using namespace std;


const unordered_map<string, TokenType> Scanner::keywords = {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}
};

Scanner::Scanner (string source) {
    this->source = source;
}

vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme.
        start = current;
        scanToken();
    }


    tokens.push_back(Token(END_OF_FILE, "", "", line));

    return tokens;
};

bool Scanner::isAtEnd(){
    return current >= source.length();
}

char Scanner::advance() {
    return source.at(current++);
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, string literal) {
    string text = source.substr(start, current-start);
    Token t(type, text, literal, line);
    tokens.push_back(t);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isdigit(c);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) {return false;}
    if (source.at(current) != expected) {return false;}

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}

void Scanner::stringFunc() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        Util::error(line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    string val = source.substr(start + 1, current - start - 2);
    addToken(STRING, val);
}


void Scanner::number(){
    while (isdigit(peek())) {
        advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peekNext())) {
        // Consume the "."
        advance();

        while (isdigit(peek())) advance();
    }

    addToken(
            NUMBER,
            source.substr(start, current-start)
    );
}

void Scanner::identifier(){
    while (isAlphaNumeric(peek())){ advance();}

    string text = source.substr(start, current-start);
    if (keywords.find(text) != keywords.end()){
        addToken(keywords.at(text));
    } else {
        addToken(IDENTIFIER);
    }
}

void Scanner::scanToken(){
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()){
                    advance();
                }
            } else {
                addToken(SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;
        case '"':
            stringFunc();
            break;
        default:
            if (isdigit(c)){
                number();
            } else if (isAlpha(c)){
                identifier();
            } else {
                Util::error(line, "Unexpected character.");
            }
            break;
    }
}