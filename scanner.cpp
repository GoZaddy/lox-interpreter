#include <string>
#include <vector>

#include "token.h"

using namespace std;


class Scanner {
    private:
        string source;
        vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

        static const unordered_map<string, TokenType> keywords;

        bool isAtEnd(){
            return current >= source.length();
        }

        char advance() {
            return source.at(current++);
        }

        void addToken(TokenType type) {
            addToken(type, NULL);
        }

        void addToken(TokenType type, string literal) {
            string text = source.substr(start, current);
            tokens.push_back(Token(type, text, literal, line));
        }

        bool isAlpha(char c) {
            return (c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                    c == '_';
        }

        bool isAlphaNumeric(char c) {
            return isAlpha(c) || isdigit(c);
        }

        bool match(char expected) {
            if (isAtEnd()) {return false;}
            if (source.at(current) != expected) {return false;}

            current++;
            return true;
        }

        char peek() {
            if (isAtEnd()) return '\0';
            return source.at(current);
        }

        char peekNext() {
            if (current + 1 >= source.length()) return '\0';
            return source.at(current + 1);
        }

        void string() {
            while (peek() != '"' && !isAtEnd()) {
                if (peek() == '\n') line++;
                advance();
            }

            if (isAtEnd()) {
                // Lox.error(line, "Unterminated string.");
                return;
            }

            // The closing ".
            advance();

            // Trim the surrounding quotes.
            string val = source.substr(start + 1, current - 1);
            addToken(STRING, val);
        }

        void number(){
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
                source.substr(start, current)
            );
        }

        void identifier(){
            while (isAlphaNumeric(peek())){ advance();}

            string text = source.substr(start, current);
            if (keywords.find(text) != keywords.end()){
                addToken(keywords[text]);
            } else {
                addToken(IDENTIFIER);
            }
        }

        void scanToken(){
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
                    string(); 
                    break;
                default:
                    if (isdigit(c)){
                        number();
                    } else if (isAlpha(c)){
                        identifier();
                    } else {
                        Lox.error(line, "Unexpected character."); // todo: fix this. probably put error() and report() in another file and import here
                    }
                    break;
            }
        }

    public:
        Scanner (string source) {
            this->source = source;
        }

        vector<Token> scanTokens() {
            while (!isAtEnd()) {
            // We are at the beginning of the next lexeme.
            start = current;
            scanToken();
            }

            tokens.push_back(Token(END_OF_FILE, "", NULL, line));
            return tokens;
        };
};

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