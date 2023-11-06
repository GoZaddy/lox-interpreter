#ifndef EXPR
#define EXPR
#include <string>
#include "token.h"
using namespace std;

template <typename T>
class Expr;

template <typename T>
class Visitor;


namespace LiteralTypeNS {
	enum LiteralType { NUMBER, STRING, TRUE, FALSE, NIL };
};

template <typename T>
class Binary: public Expr<T> {
	public:
		Expr<T>* left;
		Token operatorToken;
		Expr<T>* right;
		Binary(Expr<T>* left, Token operatorToken, Expr<T>* right) {
			this->left = left;
			this->operatorToken = operatorToken;
			this->right = right;
		}

		T accept(Visitor<T>* visitor) {
			return visitor->visit(this);
		}
};

template <typename T>
class Grouping: public Expr<T> {
	public:
		Expr<T>* expression;
		Grouping(Expr<T>* expression) {
			this->expression = expression;
		}

		T accept(Visitor<T>* visitor) {
			return visitor->visit(this);
		}
};

template <typename T>
class Literal: public Expr<T> {
	public:
		string value;
		LiteralTypeNS::LiteralType type;
		Literal(string value, LiteralTypeNS::LiteralType type) {
			this->value = value;
			this->type = type;
		}

		T accept(Visitor<T>* visitor) {
			return visitor->visit(this);
		}
};

template <typename T>
class Unary: public Expr<T> {
	public:
		Token operatorToken;
		Expr<T>* right;
		Unary(Token operatorToken, Expr<T>* right) {
			this->operatorToken = operatorToken;
			this->right = right;
		}

		T accept(Visitor<T>* visitor) {
			return visitor->visit(this);
		}
};

// visitor interface
template <typename T>
class Visitor {
	public:
		virtual T visit(Binary<T>* expr) = 0;
		virtual T visit(Grouping<T>* expr) = 0;
		virtual T visit(Literal<T>* expr) = 0;
		virtual T visit(Unary<T>* expr) = 0;
};

template <typename T>
class Expr {
	public:
		virtual T accept(Visitor<T>* visitor) = 0;
};

#endif
