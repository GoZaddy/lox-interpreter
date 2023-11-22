#ifndef RESOLVER_H
#define RESOLVER_H

#include "types.h"
#include "interpreter.h"
#include <unordered_map>
#include "helper/custom_stack.h"

typedef std::unordered_map<std::string, bool> Mapsb;

enum FunctionType {
    NONE,
    FUNCTION
};

class Resolver: ExprVisv, StmtVisv {
    private:
        Interpreter* interpreter;

        Stack<Mapsb*>* scopes;

        FunctionType currentFunction;

        void resolve(Stmtvp stmt);

        void resolve(Exprvp expr);

        void beginScope();

        void endScope();

        void declare(Token name);

        void define(Token name);

        void resolveLocal(Exprvp expr, Token name);

        void resolveFunction(Functionvp function, FunctionType type);

    public:
        Resolver(Interpreter* interpreter);

        void resolve(std::vector<Stmtvp> statements);

        rv visit(Litv* expr);

        rv visit(Logicalvp expr);

        rv visit(Groupv* expr);

        rv visit(Unav* expr);

        rv visit(Binv* expr);

        rv visit(Callvp expr);

        rv visit(Variablevp expr);

        rv visit(Assignvp expr);

        rv visit(Expressionvp stmt);

        rv visit(Functionvp stmt);

        rv visit(Ifvp stmt);

        rv visit(Printvp stmt);

        rv visit(Returnvp stmt);

        rv visit(Varvp stmt);

        rv visit(Whilevp stmt);

        rv visit(Blockvp stmt);

        rv visit(Classvp stmt); 

        rv visit(Getvp expr);

};


#endif