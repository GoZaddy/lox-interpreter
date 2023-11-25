#ifndef EXPR
#define EXPR
#include <string>
#include <vector>
#include "token.h"

using namespace std;

template <typename T>
class Expr;

template <typename T>
class ExprVisitor;


template <typename T>
class Assign;


template <typename T>
class Binary;


template <typename T>
class Call;


template <typename T>
class Get;


template <typename T>
class Grouping;


template <typename T>
class Literal;


template <typename T>
class Logical;


template <typename T>
class Set;


template <typename T>
class Super;


template <typename T>
class This;


template <typename T>
class Unary;


template <typename T>
class Variable;


template <typename T>
class Assign: public Expr<T> {
	public:
		Token name;
		Expr<T>* value;
		Assign(Token name, Expr<T>* value) {
			this->name = name;
			this->value = value;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Assign";
		}
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

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Binary";
		}
};

template <typename T>
class Call: public Expr<T> {
	public:
		Expr<T>* callee;
		Token paren;
		std::vector<Expr<T>*> arguments;
		Call(Expr<T>* callee, Token paren, std::vector<Expr<T>*> arguments) {
			this->callee = callee;
			this->paren = paren;
			this->arguments = arguments;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Call";
		}
};

template <typename T>
class Get: public Expr<T> {
	public:
		Expr<T>* object;
		Token name;
		Get(Expr<T>* object, Token name) {
			this->object = object;
			this->name = name;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Get";
		}
};

template <typename T>
class Grouping: public Expr<T> {
	public:
		Expr<T>* expression;
		Grouping(Expr<T>* expression) {
			this->expression = expression;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Grouping";
		}
};

template <typename T>
class Literal: public Expr<T> {
	public:
		string value;
		Literal(string value) {
			this->value = value;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Literal";
		}
};

template <typename T>
class Logical: public Expr<T> {
	public:
		Expr<T>* left;
		Token operatorToken;
		Expr<T>* right;
		Logical(Expr<T>* left, Token operatorToken, Expr<T>* right) {
			this->left = left;
			this->operatorToken = operatorToken;
			this->right = right;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Logical";
		}
};

template <typename T>
class Set: public Expr<T> {
	public:
		Expr<T>* object;
		Token name;
		Expr<T>* value;
		Set(Expr<T>* object, Token name, Expr<T>* value) {
			this->object = object;
			this->name = name;
			this->value = value;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Set";
		}
};

template <typename T>
class Super: public Expr<T> {
	public:
		Token keyword;
		Token method;
		Super(Token keyword, Token method) {
			this->keyword = keyword;
			this->method = method;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Super";
		}
};

template <typename T>
class This: public Expr<T> {
	public:
		Token keyword;
		This(Token keyword) {
			this->keyword = keyword;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "This";
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

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Unary";
		}
};

template <typename T>
class Variable: public Expr<T> {
	public:
		Token name;
		Variable(Token name) {
			this->name = name;
		}

		T accept(ExprVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Variable";
		}
};

// visitor interface
template <typename T>
class ExprVisitor {
	public:
		virtual T visit(Assign<T>* expr) = 0;
		virtual T visit(Binary<T>* expr) = 0;
		virtual T visit(Call<T>* expr) = 0;
		virtual T visit(Get<T>* expr) = 0;
		virtual T visit(Grouping<T>* expr) = 0;
		virtual T visit(Literal<T>* expr) = 0;
		virtual T visit(Logical<T>* expr) = 0;
		virtual T visit(Set<T>* expr) = 0;
		virtual T visit(Super<T>* expr) = 0;
		virtual T visit(This<T>* expr) = 0;
		virtual T visit(Unary<T>* expr) = 0;
		virtual T visit(Variable<T>* expr) = 0;
};

template <typename T>
class Expr {
	public:
		virtual T accept(ExprVisitor<T>* visitor) = 0;
		virtual string getType() = 0;
};

#endif
