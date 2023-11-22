#ifndef INTERPRETER_H
#define INTERPRETER_H


#include <iostream>
#include "util.h"
#include <cstdlib>
#include "types.h"
#include <vector>
#include "environment.h"
#include "native_funcs.h"
#include "lox_callable.h"
#include "lox_function.h"
#include "return.cpp"

class LoxClass;

class Interpreter : public ExprVisv, public StmtVisv{
    private:
        Environment* environment; // TODO: I wonder if this causes issues

        std::unordered_map<Exprvp, int> locals;

        rv evaluate(Exprvp expr);
        bool isTruthy(rv object);

        bool isStringLiteral(string literal);

        bool isNumberLiteral(string literal);

        bool isCallable(string expr);

        bool isClass(string expr);

        bool isInstance(string expr);

        bool isEqual(rv a, rv b);

        void checkNumberOperand(Token operatorToken, rv operand);

        void checkNumberOperands(Token operatorToken, rv left, rv right);

        string stripTrailingZeroes(rv text);

        void execute(Stmtvp stmt);

        rv lookUpVariable(Token name, Exprvp expr);

        
    public:
        Environment* globals;

        friend class LoxClass;
        

        Interpreter();
        void interpret(std::vector<Stmtvp> statements);

        void executeBlock(
            std::vector<Stmtvp> statements,
            Environment* environment
        );


        rv visit(Litv* expr);

        rv visit(Logicalvp expr);

        rv visit(Groupv* expr);

        rv visit(Unav* expr);

        rv visit(Binv* expr);

        rv visit(Callvp expr);

        rv visit(Variablevp expr);

        rv visit(Assignvp expr);

        rv visit(Expressionvp stmt);

        rv visit(Functionvp stmt);

        rv visit(Ifvp stmt);

        rv visit(Printvp stmt);

        rv visit(Returnvp stmt);

        rv visit(Varvp stmt);

        rv visit(Whilevp stmt);

        rv visit(Blockvp stmt);

        rv visit(Classvp stmt);

        void resolve(Exprvp expr, int depth);

};


#endif