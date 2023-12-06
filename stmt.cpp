#ifndef STMT
#define STMT
#include <string>
#include <vector>
#include "token.h"
#include "expr.cpp"

using namespace std;

template <typename T>
class Stmt;

template <typename T>
class StmtVisitor;


template <typename T>
class Block: public Stmt<T> {
	public:
		std::vector<Stmt<T>*> statements;
		Block(std::vector<Stmt<T>*> statements) {
			this->statements = statements;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Block";
		}
};

template <typename T>
class Expression: public Stmt<T> {
	public:
		Expr<T>* expression;
		Expression(Expr<T>* expression) {
			this->expression = expression;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Expression";
		}
};

template <typename T>
class Print: public Stmt<T> {
	public:
		Expr<T>* expression;
		Print(Expr<T>* expression) {
			this->expression = expression;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Print";
		}
};

template <typename T>
class Var: public Stmt<T> {
	public:
		Token name;
		Expr<T>* initializer;
		Var(Token name, Expr<T>* initializer) {
			this->name = name;
			this->initializer = initializer;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Var";
		}
};

// visitor interface
template <typename T>
class StmtVisitor {
	public:
		virtual T visit(Block<T>* stmt) = 0;
		virtual T visit(Expression<T>* stmt) = 0;
		virtual T visit(Print<T>* stmt) = 0;
		virtual T visit(Var<T>* stmt) = 0;
};

template <typename T>
class Stmt {
	public:
		virtual T accept(StmtVisitor<T>* visitor) = 0;
		virtual string getType() = 0;
};

#endif