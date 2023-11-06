#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <iostream>
#include <sstream>
#include "token.h"
#include "expr.cpp"

using namespace std;

class AstPrinter: public Visitor<string> {

    private:
        string parenthesize(string name, initializer_list<Expr<string>*> exprs) {
            stringstream builder;

            builder << "(" << name;
            for (auto expr : exprs) {
                builder << " " << expr->accept(this);
            }
            builder << ")";

            return builder.str();
        }
    public:
        string print(Expr<string>* expr) {
            return expr->accept(this);
        }

        

        string visit(Binary<string>* expr) {
            return parenthesize(expr->operatorToken.lexeme,
                                {expr->left, expr->right});
        }

        string visit(Grouping<string>* expr) {
            return parenthesize("group", {expr->expression});
        }

        string visit(Literal<string>* expr) {
            if (expr->value == "") return "nil";
            return expr->value;
        }

        string visit(Unary<string>* expr) {
            return parenthesize(expr->operatorToken.lexeme, {expr->right});
        }



};


// int main(){
//     Expr<string>* expr;

//     Literal<string> lit1 = Literal<string>("123", LiteralTypeNS::STRING);
//     Literal<string> lit2 = Literal<string>("45.67", LiteralTypeNS::STRING);

//     Unary<string> unary(
//         Token(MINUS, "-", "", 1),
//         &lit1
//     );

//     Token mul(STAR, "*", "", 1);

//     Grouping<string> g(
//         &lit2
//     );

//     Binary<string> b(
//         &unary,
//         mul,
//         &g
//     );

//     expr = &b;

//     AstPrinter astp;

//     cout << astp.print(expr) << endl;
    
// }

#endif