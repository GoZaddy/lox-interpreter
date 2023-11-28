#ifndef TYPES_H
#define TYPES_H

#include "stmt.cpp"
#include "expr.cpp"
#include <variant>
#include "helper/object.cpp"



typedef Object* rv; // rv - return value for visitor
typedef void stmt_rv;

// extern rv null;




typedef ExprVisitor<rv> ExprVisv;
typedef StmtVisitor<stmt_rv, rv> StmtVisv;
typedef Literal<rv> Litv;
typedef Literal<rv>* Litvp;
typedef Logical<rv> Logicalv;
typedef Logicalv* Logicalvp;
typedef Grouping<rv> Groupv;
typedef Grouping<rv>* Groupvp;
typedef Get<rv> Getv;
typedef Getv* Getvp;
typedef Set<rv> Setv;
typedef Setv* Setvp;
typedef Expr<rv> Exprv;
typedef Expr<rv>* Exprvp;
typedef Unary<rv> Unav;
typedef Unary<rv>* Unavp;
typedef Binary<rv> Binv;
typedef Binary<rv>* Binvp;
typedef Call<rv> Callv;
typedef Callv* Callvp;
typedef This<rv> Thisv;
typedef Thisv* Thisvp;
typedef Super<rv> Superv;
typedef Superv* Supervp;
typedef Variable<rv> Variablev;
typedef Variablev* Variablevp;
typedef Assign<rv> Assignv;
typedef Assign<rv>* Assignvp;


typedef Stmt<stmt_rv> Stmtv;
typedef Stmtv* Stmtvp;
typedef Expression<stmt_rv, rv> Expressionv;
typedef Expressionv* Expressionvp;
typedef Print<stmt_rv, rv> Printv;
typedef Printv* Printvp;
typedef Var<stmt_rv, rv> Varv;
typedef Varv* Varvp;
typedef Block<stmt_rv> Blockv;
typedef Blockv* Blockvp;
typedef If<stmt_rv, rv> Ifv;
typedef Ifv* Ifvp;
typedef While<stmt_rv, rv> Whilev;
typedef Whilev* Whilevp;
typedef Function<stmt_rv> Functionv;
typedef Functionv* Functionvp;
typedef Return<stmt_rv, rv> Returnv;
typedef Returnv* Returnvp;
typedef Class<stmt_rv> Classv;
typedef Classv* Classvp;


#endif
