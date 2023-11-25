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
            if (current == 0) {cerr << "invalid call to previous() because current is 0" << endl;}
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
            return assignment();
        }

        Stmtvp declaration(){
            try {
                if (match({CLASS})) return classDeclaration();
                if (match({FUN})) return function_stmt("function");
                if (match({VAR})) return varDeclaration();

                return statement();
            } catch (string error){
                synchronize();
                return nullptr;
            }
        }

        Stmtvp classDeclaration() {
            Token name = consume(IDENTIFIER, "Expect class name.");
            consume(LEFT_BRACE, "Expect '{' before class body.");

            std::vector<Functionvp> methods;
            while (!check(RIGHT_BRACE) && !isAtEnd()) {
                methods.push_back((Functionvp) function_stmt("method"));
            }

            consume(RIGHT_BRACE, "Expect '}' after class body.");

            Classvp res = new Classv(name, methods);
            return res;
        }

        Stmtvp statement() {
            if (match({FOR})) return forStatement();
            if (match({IF})) return ifStatement();
            if (match({PRINT})) return printStatement();
            if (match({RETURN})) return returnStatement();
            if (match({WHILE})) return whileStatement();
            if (match({LEFT_BRACE})) return new Blockv(block());
            
            return expressionStatement();
        }

        Stmtvp forStatement() {
            consume(LEFT_PAREN, "Expect '(' after 'for'.");

            Stmtvp initializer;
            if (match({SEMICOLON})) {
                initializer = nullptr;
            } else if (match({VAR})) {
                initializer = varDeclaration();
            } else {
                initializer = expressionStatement();
            }


            Exprvp condition = nullptr;
            if (!check(SEMICOLON)) {
                condition = expression();
            }
            consume(SEMICOLON, "Expect ';' after loop condition.");

            Exprvp increment = nullptr;
            if (!check(RIGHT_PAREN)) {
            increment = expression();
            }
            consume(RIGHT_PAREN, "Expect ')' after for clauses.");


            Stmtvp body = statement();

            // desugaring
            std::vector<Stmtvp> statements;
            if (increment != nullptr) {
                statements = {
                    body,
                    new Expressionv(increment)
                };
                
                body = new Blockv(statements);
            }


            if (condition == nullptr){
                condition = new Litv("true");
            }
            body = new Whilev(condition, body);

            if (initializer != nullptr) {
                statements = {initializer, body};
                body = new Blockv(statements);
            }

            return body;
        }

        Stmtvp ifStatement() {
            consume(LEFT_PAREN, "Expect '(' after 'if'.");
            Exprvp condition = expression();
            consume(RIGHT_PAREN, "Expect ')' after if condition."); 

            Stmtvp thenBranch = statement();
            Stmtvp elseBranch = nullptr;
            if (match({ELSE})) {
                elseBranch = statement();
            }

            return new Ifv(condition, thenBranch, elseBranch);
        }

        Stmtvp printStatement() {
            Exprvp value = expression();
            consume(SEMICOLON, "Expect ';' after value.");
            Stmtvp stmt = new Printv(value);
            return stmt;
        }

        Stmtvp returnStatement() {
            Token keyword = previous();
            Exprvp value = nullptr;
            if (!check(SEMICOLON)) {
                value = expression();
            }

            consume(SEMICOLON, "Expect ';' after return value.");
            return new Returnv(keyword, value);
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

        Stmtvp whileStatement() {
            consume(LEFT_PAREN, "Expect '(' after 'while'.");
            Exprvp condition = expression();
            consume(RIGHT_PAREN, "Expect ')' after condition.");
            Stmtvp body = statement();

            return new Whilev(condition, body);
        }

        Stmtvp expressionStatement() {
            Exprvp expr = expression();
            consume(SEMICOLON, "Expect ';' after expression.");
            Stmtvp stmt = new Expressionv(expr);
            return stmt;
        }

        Functionvp function_stmt(string kind) {
            Token name = consume(IDENTIFIER, "Expect " + kind + " name.");

            consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
            vector<Token> parameters;
            if (!check(RIGHT_PAREN)) {
                do {
                    if (parameters.size() >= 255) {
                        Util::error(peek(), "Can't have more than 255 parameters.");
                    }

                    parameters.push_back(
                        consume(IDENTIFIER, "Expect parameter name.")
                    );
                } while (match({COMMA}));
            }
            consume(RIGHT_PAREN, "Expect ')' after parameters.");

            consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
            vector<Stmtvp> body = block();
            return new Functionv(name, parameters, body);
        }

        vector<Stmtvp> block() {
            vector<Stmtvp> statements;

            while (!check(RIGHT_BRACE) && !isAtEnd()) {
                statements.push_back(declaration());
            }

            consume(RIGHT_BRACE, "Expect '}' after block.");
            return statements;
        }

        Exprvp assignment(){
            Exprvp expr = or_expr();

            if (match({EQUAL})) {
                Token equals = previous();
                Exprvp value = assignment();

                if (expr->getType() == "Variable") {
                    Token name = ((Variablevp) expr)->name;
                    return new Assignv(name, value);
                } else if (expr->getType() == "Get"){
                    Getvp get = (Getvp)expr;
                    return new Setv(get->object, get->name, value);
                }

                Util::error(equals, "Invalid assignment target."); 
            }

            return expr;
        }

        Exprvp or_expr() {
            Exprvp expr = and_expr();

            while (match({OR})) {
                Token operatorToken = previous();
                Exprvp right = and_expr();
                expr = new Logicalv(expr, operatorToken, right);
            }

            return expr;
        }

        Exprvp and_expr() {
            Exprvp expr = equality();

            while (match({AND})) {
                Token operatorToken = previous();
                Exprvp right = equality();
                expr = new Logicalv(expr, operatorToken, right);
            }

            return expr;
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

            return call_expr();
        }

        Exprvp finishCall(Exprvp callee) {
            vector<Exprvp> arguments;
            if (!check(RIGHT_PAREN)) {
                do {
                    if (arguments.size() >= 255) {
                        Util::error(peek(), "Can't have more than 255 arguments.");
                    }
                    arguments.push_back(expression());
                } while (match({COMMA}));
            }

            Token paren = consume(RIGHT_PAREN,  "Expect ')' after arguments.");

            return new Callv(callee, paren, arguments);
        }

        Exprvp call_expr() {
            Exprvp expr = primary();

            while (true) { 
                if (match({LEFT_PAREN})) {
                    expr = finishCall(expr);
                } else if (match({DOT})){
                    Token name = consume(IDENTIFIER,
                        "Expect property name after '.'.");
                    expr = new Getv(expr, name);
                } else {
                    break;
                }
            }

            return expr;
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

            if (match({THIS})) return new Thisv(previous());

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