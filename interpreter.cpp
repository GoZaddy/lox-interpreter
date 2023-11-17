#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include "expr.cpp"
#include "util.h"
#include <cstdlib>
#include "types.h"
#include <vector>

namespace Interpreter {
    class Interpreter : public ExprVisv, public StmtVisv{
        private:
            rv evaluate(Exprvp expr){
                return expr->accept(this);
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

            string stringify(rv text){
                if (text.size() > 7 && text.substr(text.size()-7) == ".000000"){
                    text = text.substr(0, text.size()-7);
                }
                return text;
            }

            void execute(Stmtvp stmt){
                stmt->accept(this);
            }

            
        public:
            void interpret(std::vector<Stmtvp> statements){
                try{
                    for (int i = 0; i < statements.size(); ++i){
                        execute(statements[i]);
                        // delete statements[i]; // i was testing something
                    }
                } catch(string error){std::cerr << error << endl;};
            }
            rv visit(Litv* expr){
                return expr->value;
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
                        return std::to_string(Util::doub(left) - Util::doub(right));
                    case SLASH:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return std::to_string(Util::doub(left) / Util::doub(right));
                    case STAR:
                        checkNumberOperands(expr->operatorToken, left, right);
                        return std::to_string(Util::doub(left) * Util::doub(right));
                    case PLUS:
                        if (isStringLiteral(left) && isStringLiteral(right)){
                            left.pop_back();
                            return left + right.substr(1);
                        }
                        
                        if (isNumberLiteral(left) && isNumberLiteral(right)){
                            return std::to_string(Util::doub(left) + Util::doub(right));
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

            rv visit(Expressionvp stmt){
                return evaluate(stmt->expression);
            }

            rv visit(Printvp stmt){
                rv value = evaluate(stmt->expression);
                std::cout << value << endl;
                return value;
            }


    };
};

#endif