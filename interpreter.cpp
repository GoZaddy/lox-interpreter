#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include "expr.cpp"
#include "util.h"
#include <cstdlib>
#include "types.h"
#include <vector>
#include "environment.cpp"
#include "native_funcs.cpp"
#include "lox_function.cpp"

namespace Interpreter {
    class Interpreter : public ExprVisv, public StmtVisv{
        private:
            Environment* environment = globals; // TODO: I wonder if this causes issues

            rv evaluate(Exprvp expr){
                rv value = expr->accept(this);
                // delete expr; // TODO: test this out
                return value;
            }
            bool isTruthy(rv object){
                if (object == "nil") return false; // TODO: what other values could return false
                if (object == "true") return true;
                if (object == "false") return false;
                return true;
            }

            bool isStringLiteral(string literal){
                return literal[0] == '\"' && literal[literal.size()-1] == '\"';
            }

            bool isNumberLiteral(string literal){
                char* end = nullptr;
                double val = strtod(literal.c_str(), &end);
                return end != literal.c_str() && *end == '\0';
            }

            bool isEqual(rv a, rv b){
                return a == b;
            }

            void checkNumberOperand(Token operatorToken, rv operand){
                if (isNumberLiteral(operand)) return;
                throw Util::runtimeError(operatorToken, "Operand must be a number.");
            }

            void checkNumberOperands(Token operatorToken, rv left, rv right){
                if (isNumberLiteral(left) && isNumberLiteral(right)) return;
                throw Util::runtimeError(operatorToken, "Operands must be numbers");
            }

            string stripTrailingZeroes(rv text){
                if (text.size() > 7 && text.substr(text.size()-7) == ".000000"){
                    text = text.substr(0, text.size()-7);
                }
                return text;
            }

            void execute(Stmtvp stmt){
                stmt->accept(this);
            }

            

            
        public:
            Environment* globals = new Environment();
            

            Interpreter() {
                Clock clockFunc;
                globals->define("clock", clockFunc); // TODO: figure this out, consider using a new map in Environment for just functions
            }
            void interpret(std::vector<Stmtvp> statements){
                try{
                    for (int i = 0; i < statements.size(); ++i){
                        execute(statements[i]);
                        // delete statements[i]; // i was testing something
                    }
                } catch(string error){std::cerr << error << endl;};
            }

            void executeBlock(
                std::vector<Stmtvp> statements,
                Environment* environment
            ) {
                Environment* previous = this->environment;
                try {
                    this->environment = environment;

                    for (auto statement : statements) {
                        execute(statement);
                    }
                } 
                catch(string error){std::cerr << error << endl;}
                catch (...){std:cerr << "unrecognized error occurred" << endl;}
                
                this->environment = previous;
                
            }


            rv visit(Litv* expr){
                return expr->value;
            }

            rv visit(Logicalvp expr) {
                rv left = evaluate(expr->left);

                if (expr->operatorToken.type == OR) {
                    if (isTruthy(left)) return left;
                } else {
                    if (!isTruthy(left)) return left;
                }

                return evaluate(expr->right);
            }

            rv visit(Groupv* expr){
                return evaluate(expr->expression);
            }

            rv visit(Unav* expr){
                rv right = evaluate(expr->right);

                switch(expr->operatorToken.type){
                    case MINUS:
                        checkNumberOperand(expr->operatorToken, right);
                        return "-"+right;
                    case BANG:
                        return isTruthy(right) ? "false" : "true";
                    default:
                        //unreachable
                        return ""; // empty string is null for us
                }
            }

            rv visit(Binv* expr){
                rv left = evaluate(expr->left);
                rv right = evaluate(expr->right);

                switch(expr->operatorToken.type){
                    case MINUS:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return stripTrailingZeroes(std::to_string(Util::doub(left) - Util::doub(right)));
                    case SLASH:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return stripTrailingZeroes(std::to_string(Util::doub(left) / Util::doub(right)));
                    case STAR:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return stripTrailingZeroes(std::to_string(Util::doub(left) * Util::doub(right)));
                    case PLUS:
                        if (isStringLiteral(left) && isStringLiteral(right)){
                            left.pop_back();
                            return left + right.substr(1);
                        }
                        
                        if (isNumberLiteral(left) && isNumberLiteral(right)){
                            return stripTrailingZeroes(std::to_string(Util::doub(left) + Util::doub(right)));
                        }

                        throw Util::runtimeError(expr->operatorToken, "Operands must be two numbers or two strings.");
                        
                    case GREATER:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return (Util::doub(left) > Util::doub(right)) ? "true" : "false";
                    case GREATER_EQUAL:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return (Util::doub(left) >= Util::doub(right)) ? "true" : "false";
                    case LESS:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return (Util::doub(left) < Util::doub(right)) ? "true" : "false";
                    case LESS_EQUAL:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return (Util::doub(left) <= Util::doub(right)) ? "true" : "false";
                        
                    case BANG_EQUAL:
                        return isEqual(left, right) ? "false" : "true";
                    case EQUAL_EQUAL:
                        return isEqual(left, right) ? "true" : "false";
                        

                    default:
                        //unreachable
                        return "";
                }
            }

            rv visit(Callvp expr) {
                rv callee = evaluate(expr->callee);

                std::vector<rv> arguments;
                for (auto argument : expr->arguments) { 
                    arguments.push_back(evaluate(argument));
                }


                LoxCallable func = callee; // figure this out

                if (arguments.size() != func.arity()) {
                    throw Util::runtimeError(expr->paren, "Expected " +
                        func.arity() + " arguments but got " +
                        arguments.size() + ".");
                }
                return func.call(this, arguments);
            }

            rv visit(Variablevp expr){
                return environment->get(expr->name);
            }

            rv visit(Assignvp expr){
                rv value = evaluate(expr->value);
                environment->assign(expr->name, value);
                return value;
            }

            rv visit(Expressionvp stmt){
                return evaluate(stmt->expression);
            }

            rv visit(Functionvp stmt){
                LoxFunction* function = new LoxFunction(stmt);
                environment->define(stmt->name.lexeme, function); // come up with our solution for this
                return "";
            }

            rv visit(Ifvp stmt) {
                if (isTruthy(evaluate(stmt->condition))) {
                    execute(stmt->thenBranch);
                } else if (stmt->elseBranch != nullptr) {
                    execute(stmt->elseBranch);
                }
                return "";
            }

            rv visit(Printvp stmt){
                rv value = evaluate(stmt->expression);
                std::cout << value << endl;
                return "";
            }

            rv visit(Varvp stmt){
                string value = "";
                if (stmt->initializer != nullptr){
                    value = evaluate(stmt->initializer);
                }

                environment->define(stmt->name.lexeme, value);
                return value;
            }

            rv visit(Whilevp stmt) {
                while (isTruthy(evaluate(stmt->condition))) {
                    execute(stmt->body);
                }
                return "";
            }

            rv visit(Blockvp stmt) {
                Environment* newEnv = new Environment(environment); 
                executeBlock(stmt->statements, newEnv);
                delete newEnv; // free memory
                return "";
            }

            


    };
};

#endif