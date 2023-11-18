#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>

#include "token.h"
#include "expr.cpp"
#include "util.h"
#include "stmt.cpp"

#include "types.h"

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

        Exprvp expression(){
            return equality();
        }

        Stmtvp declaration(){
            try {
                if (match({VAR})) return varDeclaration();

                return statement();
            } catch (string error){
                synchronize();
                return nullptr;
            }
        }

        Stmtvp statement() {
            if (match({PRINT})) return printStatement();

            return expressionStatement();
        }

        Stmtvp printStatement() {
            Exprvp value = expression();
            consume(SEMICOLON, "Expect ';' after value.");
            Stmtvp stmt = new Printv(value);
            return stmt;
        }

        Stmtvp varDeclaration(){
            Token name = consume(IDENTIFIER, "Expect variable name.");

            Exprvp initializer = nullptr;
            if (match({EQUAL})){
                initializer = expression();
            }

            consume(SEMICOLON, "Expect ';' after variable declaration.");
            return new Varv(name, initializer);
        }

        Stmtvp expressionStatement() {
            Exprvp expr = expression();
            consume(SEMICOLON, "Expect ';' after expression.");
            Stmtvp stmt = new Expressionv(expr);
            return stmt;
        }

        Exprvp equality(){
            Exprvp expr = comparison();

            while(match({BANG_EQUAL, EQUAL_EQUAL})){
                Token operatorToken = previous();
                Exprvp right = comparison();
                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }

        Exprvp comparison() {
            Exprvp expr = term();

            while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})){
                Token operatorToken = previous();

                Exprvp right = term();

                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }

        Exprvp term(){
            Exprvp expr = factor();

            while(match({MINUS, PLUS})){
                Token operatorToken = previous();

                Exprvp right = factor();

                Binv* temp = new Binv(expr, operatorToken, right);
                
                expr = temp;
            }

        

            return expr;
        };

        Exprvp factor(){
            Exprvp expr = unary();

            while(match({SLASH, STAR})){
                Token operatorToken = previous();

                Exprvp right = unary();

                Binv* temp = new Binv(expr, operatorToken, right);
                expr = temp;
            }

            return expr;
        }; 


        Exprvp unary(){
            Exprvp res;
            if (match({BANG, MINUS})){
                Token operatorToken = previous();
                Exprvp right = unary();

                Unav* unav = new Unav(operatorToken, right);
                res = unav;
                return res;
            }

            return primary();
        }

        Exprvp primary(){
            Exprvp res;
            if (match({FALSE})) {
                Litv* temp = new Litv("false");
                res = temp;
                return res;
            }

            if (match({TRUE})) {
                Litv* temp = new Litv("true");
                res = temp;
                return res;
            }

            if (match({NIL})) {
                Litv* temp = new Litv("nil");
                res = temp;
                return res;
            }

            if (match({NUMBER, STRING})) {
                Litv* temp = new Litv(previous().literal);
                res = temp;

                return res;
            }

            if (match({IDENTIFIER})){
                res = new Variablev(previous());
                return res;
            }

            if (match({LEFT_PAREN})) {
                Exprvp expr = expression();
                consume(RIGHT_PAREN, "Expect ')' after expression.");

                Groupv* temp = new Groupv(expr);
                res = temp;
                return res;
            }

            throw parseError(peek(), "Expect expression");
            return nullptr;
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

        vector<Stmtvp> parse(){
            vector<Stmtvp> statements;
            try{
                while (!isAtEnd()){
                    statements.push_back(declaration());
                }
            } catch(string error){
                std::cerr << error << endl;
            }
            
            

            return statements;
        }
};


#endif