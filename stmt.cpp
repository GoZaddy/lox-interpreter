#ifndef STMT
#define STMT
#include <string>
#include <vector>
#include "token.h"
#include "expr.cpp"

using namespace std;

template <typename T>
class Stmt;

template <typename T, typename P>
class StmtVisitor;


template <typename T>
class Block;


template <typename T>
class Class;


template <typename T, typename P>
class Expression;


template <typename T>
class Function;


template <typename T, typename P>
class If;


template <typename T, typename P>
class Print;


template <typename T, typename P>
class Return;


template <typename T, typename P>
class Var;


template <typename T, typename P>
class While;


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
class Class: public Stmt<T> {
	public:
		Token name;
		Variable<T>* superclass;
		std::vector<Function<T>*> methods;
		Class(Token name, Variable<T>* superclass, std::vector<Function<T>*> methods) {
			this->name = name;
			this->superclass = superclass;
			this->methods = methods;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Class";
		}
};

template <typename T, typename P>
class Expression: public Stmt<T> {
	public:
		Expr<P>* expression;
		Expression(Expr<P>* expression) {
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

template <typename T, typename P>
class If: public Stmt<T> {
	public:
		Expr<P>* condition;
		Stmt<T>* thenBranch;
		Stmt<T>* elseBranch;
		If(Expr<P>* condition, Stmt<T>* thenBranch, Stmt<T>* elseBranch) {
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

template <typename T, typename P>
class Print: public Stmt<T> {
	public:
		Expr<P>* expression;
		Print(Expr<P>* expression) {
			this->expression = expression;
		}

		T accept(StmtVisitor<T>* visitor) {
			return visitor->visit(this);
		}

		string getType() {
			return "Print";
		}
};

template <typename T, typename P>
class Return: public Stmt<T> {
	public:
		Token keyword;
		Expr<P>* value;
		Return(Token keyword, Expr<P>* value) {
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

template <typename T, typename P>
class Var: public Stmt<T> {
	public:
		Token name;
		Expr<P>* initializer;
		Var(Token name, Expr<P>* initializer) {
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

template <typename T, typename P>
class While: public Stmt<T> {
	public:
		Expr<P>* condition;
		Stmt<T>* body;
		While(Expr<P>* condition, Stmt<T>* body) {
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
template <typename T, typename P>
class StmtVisitor {
	public:
		virtual T visit(Block<T>* stmt) = 0;
		virtual T visit(Class<T>* stmt) = 0;
		virtual T visit(Expression<T, P>* stmt) = 0;
		virtual T visit(Function<T>* stmt) = 0;
		virtual T visit(If<T, P>* stmt) = 0;
		virtual T visit(Print<T,P>* stmt) = 0;
		virtual T visit(Return<T,P>* stmt) = 0;
		virtual T visit(Var<T,P>* stmt) = 0;
		virtual T visit(While<T,P>* stmt) = 0;
};

template <typename T>
class Stmt {
	public:
		virtual T accept(StmtVisitor<T>* visitor) = 0;
		virtual string getType() = 0;
};

#endif
