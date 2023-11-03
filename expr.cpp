#include <string>
#include "token.h"
using namespace std;

class Expr {
};

// visitor interface
template <typename T>
class Visitor {
	T visitBinaryExpr(Binary expr) = 0;
	T visitGroupingExpr(Grouping expr) = 0;
	T visitLiteralExpr(Literal expr) = 0;
	T visitUnaryExpr(Unary expr) = 0;
}

class Binary: public Expr{
	public:
		Expr left;
		Token operatorToken;
		Expr right;
		Binary(Expr left, Token operatorToken, Expr right) {
			this->left = left;
			this->operatorToken = operatorToken;
			this->right = right;
		}
};

class Grouping: public Expr{
	public:
		Expr expression;
		Grouping(Expr expression) {
			this->expression = expression;
		}
};

class Literal: public Expr{
	public:
		string value;
		Literal(string value) {
			this->value = value;
		}
};

class Unary: public Expr{
	public:
		Token operatorToken;
		Expr right;
		Unary(Token operatorToken, Expr right) {
			this->operatorToken = operatorToken;
			this->right = right;
		}
};

