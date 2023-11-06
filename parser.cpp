#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>

#include "token.h"
#include "expr.cpp"
#include "util.h"


typedef Expr<string>* Exprv;
typedef Binary<string> Binv;
typedef Unary<string> Unav;
typedef Literal<string> Litv;
typedef Grouping<string> Groupv;

class Parser {
    private:
        std::vector<Token> tokens;
        int current = 0;


        bool isAtEnd(){
            return peek().type == END_OF_FILE;
        }

        Token peek(){
            return tokens[current];
        }

        Token previous(){
            if (current == 0) {cout << "invalid call to previous() because current is 0" << endl;}
            return tokens[current-1];
        }

        bool check(TokenType type){
            if (isAtEnd()) return false;
            return peek().type == type;
        }

        Token advance(){
            if (!isAtEnd()) current++;
            return previous();
        }

        bool match(initializer_list<TokenType> types){
            for (auto type : types){
                if (check(type)){
                    advance();
                    return true;
                }
            }

            return false;
        }

        string parseError(Token token, string message){
            Util::error(token, message);
            return "Parse Error occurred!";
        }

        Exprv expression(){
            return equality();
        }

        Exprv equality(){
            Exprv expr = comparison();

            while(match({BANG_EQUAL, EQUAL_EQUAL})){
                Token operatorToken = previous();
                Exprv right = comparison();
                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }

        Exprv comparison() {
            Exprv expr = term();

            while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})){
                Token operatorToken = previous();

                Exprv right = term();

                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }

        Exprv term(){
            Exprv expr = factor();

            while(match({MINUS, PLUS})){
                Token operatorToken = previous();

                Exprv right = factor();

                Binv* temp = new Binv(expr, operatorToken, right);
                
                expr = temp;
            }

        

            return expr;
        };

        Exprv factor(){
            Exprv expr = unary();

            while(match({SLASH, STAR})){
                Token operatorToken = previous();

                Exprv right = unary();

                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }; 


        Exprv unary(){
            Exprv res;
            if (match({BANG, MINUS})){
                Token operatorToken = previous();
                Exprv right = unary();

                Unav* unav = new Unav(operatorToken, right);
                res = unav;
                return res;
            }

            return primary();
        }

        Exprv primary(){
            Exprv res;
            if (match({FALSE})) {
                Litv* temp = new Litv("", LiteralTypeNS::FALSE);
                res = temp;
                return res;
            }

            if (match({TRUE})) {
                Litv* temp = new Litv("", LiteralTypeNS::TRUE);
                res = temp;
                return res;
            }

            if (match({NIL})) {
                Litv* temp = new Litv("", LiteralTypeNS::NIL);
                res = temp;
                return res;
            }

            if (match({NUMBER, STRING})) {
                LiteralTypeNS::LiteralType lt;
                
                if (previous().type == NUMBER) lt = LiteralTypeNS::NUMBER;
                if (previous().type == STRING) lt = LiteralTypeNS::STRING;

                Litv* temp = new Litv(previous().literal, lt);
                res = temp;

                return res;
            }

            if (match({LEFT_PAREN})) {
                Exprv expr = expression();
                consume(RIGHT_PAREN, "Expect ')' after expression.");

                Groupv* temp = new Groupv(expr);
                res = temp;
                return res;
            }

            throw parseError(peek(), "Expect expression");
        }

        Token consume(TokenType type, string message){
            if (check(type)) return advance();

            throw parseError(peek(), message);
        }

        void synchronize(){
            advance();

            while (!isAtEnd()){
                if (previous().type == SEMICOLON) return;

                switch (peek().type){
                    case CLASS:
                    case FUN:
                    case VAR:
                    case FOR:
                    case IF:
                    case WHILE:
                    case PRINT:
                    case RETURN:
                        return;
                }

                advance();
            }
        }

    public:
        Parser(std::vector<Token> tokens){
            this->tokens = tokens;
        }

        Exprv parse(){
            try {
                return expression();
            } catch (string message){
                return nullptr;
            }
        }
};


#endif