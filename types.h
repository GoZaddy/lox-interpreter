#ifndef TYPES_H
#define TYPES_H

#include "stmt.cpp"
#include "expr.cpp"

typedef string rv; // rv - return value for visitor
typedef ExprVisitor<rv> ExprVisv;
typedef StmtVisitor<rv> StmtVisv;
typedef Literal<rv> Litv;
typedef Grouping<rv> Groupv;
typedef Expr<rv> Exprv;
typedef Expr<rv>* Exprvp;
typedef Unary<rv> Unav;
typedef Binary<rv> Binv;
typedef Stmt<rv> Stmtv;
typedef Stmt<rv>* Stmtvp;
typedef Expression<rv> Expressionv;
typedef Print<rv> Printv;



#endif
