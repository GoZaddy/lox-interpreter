#ifndef TYPES_H
#define TYPES_H

#include "stmt.cpp"
#include "expr.cpp"

typedef string rv; // rv - return value for visitor
typedef ExprVisitor<rv> ExprVisv;
typedef StmtVisitor<rv> StmtVisv;
typedef Literal<rv> Litv;
typedef Logical<rv> Logicalv;
typedef Logicalv* Logicalvp;
typedef Grouping<rv> Groupv;
typedef Expr<rv> Exprv;
typedef Expr<rv>* Exprvp;
typedef Unary<rv> Unav;
typedef Binary<rv> Binv;
typedef Call<rv> Callv;
typedef Callv* Callvp;
typedef Variable<rv> Variablev;
typedef Variablev* Variablevp;
typedef Assign<rv> Assignv;
typedef Assign<rv>* Assignvp;
typedef Stmt<rv> Stmtv;
typedef Stmtv* Stmtvp;
typedef Expression<rv> Expressionv;
typedef Expressionv* Expressionvp;
typedef Print<rv> Printv;
typedef Printv* Printvp;
typedef Var<rv> Varv;
typedef Varv* Varvp;
typedef Block<rv> Blockv;
typedef Blockv* Blockvp;
typedef If<rv> Ifv;
typedef Ifv* Ifvp;
typedef While<rv> Whilev;
typedef Whilev* Whilevp;
typedef Function<rv> Functionv;
typedef Functionv* Functionvp;


#endif
