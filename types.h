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
typedef Variable<rv> Variablev;
typedef Variablev* Variablevp;
typedef Stmt<rv> Stmtv;
typedef Stmtv* Stmtvp;
typedef Expression<rv> Expressionv;
typedef Expressionv* Expressionvp;
typedef Print<rv> Printv;
typedef Printv* Printvp;
typedef Var<rv> Varv;
typedef Varv* Varvp;



#endif
