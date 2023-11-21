#include "resolver.h"



rv null = "";

Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
    this->currentFunction = NONE;
    this->scopes = new Stack<Mapsb*>();
}

void Resolver::resolve(Stmtvp stmt){
    stmt->accept(this);
}

void Resolver::resolve(Exprvp expr){
    expr->accept(this);
}

void Resolver::resolve(std::vector<Stmtvp> statements){
    for (auto statement : statements) {
        resolve(statement);
    }
}

void Resolver::beginScope(){
    Mapsb* map = new Mapsb();
    scopes->push(map);
}

void Resolver::endScope(){
    scopes->pop();
}

rv Resolver::visit(Blockvp stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return null;
}

rv Resolver::visit(Varvp stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return null;
}

void Resolver::declare(Token name) {
    if (scopes->empty()) return;

    Mapsb* scope = scopes->peek();
    if (scope->find(name.lexeme) != scope->end()){
        Util::error(name, "Already a variable with this name in this scope.");
    } else {
        (*scope)[name.lexeme] =  false;
    }
    
}

void Resolver::define(Token name) {
    if (scopes->empty()) return;
    (*(scopes->peek()))[name.lexeme] = true;
}

rv Resolver::visit(Variablevp expr) {
    if (
        !scopes->empty() &&
        (
            scopes->peek()->find(expr->name.lexeme) != scopes->peek()->end() &&
            scopes->peek()->at(expr->name.lexeme) == false
        )
    ) {
        Util::error(expr->name,
            "Can't read local variable in its own initializer.");
    }

    resolveLocal(expr, expr->name);
    return null;
}

void Resolver::resolveLocal(Exprvp expr, Token name) {
    Mapsb* currScope = scopes->peek();
    for (int i = scopes->size() - 1; i >= 0; i--) {
        // TODO: MIGHT BE BUGGY - JUST INTUITION
        if (scopes->at(i)->find(name.lexeme) != scopes->at(i)->end()) {
            interpreter->resolve(expr, scopes->size() - 1 - i);
            return;
        }
        
    }
}

rv Resolver::visit(Assignvp expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return null;
}

rv Resolver::visit(Functionvp stmt){
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION);
    return null;
}



void Resolver::resolveFunction(Functionvp func, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto param : func->params) {
      declare(param);
      define(param);
    }
    resolve(func->body);
    endScope();

    currentFunction = enclosingFunction;
}

rv Resolver::visit(Expressionvp stmt) {
    resolve(stmt->expression);
    return null;
}

rv Resolver::visit(Ifvp stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
    return null;
}

rv Resolver::visit(Printvp stmt) {
    resolve(stmt->expression);
    return null;
}

rv Resolver::visit(Returnvp stmt) {
    if (currentFunction == NONE) {
      Util::error(stmt->keyword, "Can't return from top-level code.");
    }

    if (stmt->value != nullptr) {
      resolve(stmt->value);
    }

    return null;
}

rv Resolver::visit(Whilevp stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return null;
}

rv Resolver::visit(Binvp expr) {
    resolve(expr->left);
    resolve(expr->right);
    return null;
}


rv Resolver:: visit(Callvp expr) {
    resolve(expr->callee);

    for (auto argument : expr->arguments) {
      resolve(argument);
    }

    return null;
}

rv Resolver:: visit(Groupvp expr) {
    resolve(expr->expression);
    return null;
}

rv Resolver:: visit(Litvp expr) {
    return null;
}

rv Resolver:: visit(Logicalvp expr) {
    resolve(expr->left);
    resolve(expr->right);
    return null;
}

rv Resolver:: visit(Unavp expr) {
    resolve(expr->right);
    return null;
}

