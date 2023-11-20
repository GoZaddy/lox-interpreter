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
class Function: public Stmt<T> {
	public:
		Token name;
		std::vector<Token> params;
		std::vector<Stmt<T>*> body;
		Function(Token name, std::vector<Token> params, std::vector<Stmt<T>*> body) {
			this->name = name;
			this->params = params;
			this->body = body;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Function";
		}
};

template <typename T>
class If: public Stmt<T> {
	public:
		Expr<T>* condition;
		Stmt<T>* thenBranch;
		Stmt<T>* elseBranch;
		If(Expr<T>* condition, Stmt<T>* thenBranch, Stmt<T>* elseBranch) {
			this->condition = condition;
			this->thenBranch = thenBranch;
			this->elseBranch = elseBranch;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "If";
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
class Return: public Stmt<T> {
	public:
		Token keyword;
		Expr<T>* value;
		Return(Token keyword, Expr<T>* value) {
			this->keyword = keyword;
			this->value = value;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Return";
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

template <typename T>
class While: public Stmt<T> {
	public:
		Expr<T>* condition;
		Stmt<T>* body;
		While(Expr<T>* condition, Stmt<T>* body) {
			this->condition = condition;
			this->body = body;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "While";
		}
};

// visitor interface
template <typename T>
class StmtVisitor {
	public:
		virtual T visit(Block<T>* stmt) = 0;
		virtual T visit(Expression<T>* stmt) = 0;
		virtual T visit(Function<T>* stmt) = 0;
		virtual T visit(If<T>* stmt) = 0;
		virtual T visit(Print<T>* stmt) = 0;
		virtual T visit(Return<T>* stmt) = 0;
		virtual T visit(Var<T>* stmt) = 0;
		virtual T visit(While<T>* stmt) = 0;
};

template <typename T>
class Stmt {
	public:
		virtual T accept(StmtVisitor<T>* visitor) = 0;
		virtual string getType() = 0;
};

#endif
