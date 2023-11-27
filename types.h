#ifndef TYPES_H
#define TYPES_H

#include "stmt.cpp"
#include "expr.cpp"
#include <variant>



typedef std::variant<std::string, double, bool, void*> rv; // rv - return value for visitor

extern rv null;




typedef ExprVisitor<rv> ExprVisv;
typedef StmtVisitor<rv> StmtVisv;
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
typedef Return<rv> Returnv;
typedef Returnv* Returnvp;
typedef Class<rv> Classv;
typedef Classv* Classvp;


#endif
