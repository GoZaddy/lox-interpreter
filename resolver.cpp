// #include "resolver.h"

#include "declr.h"


Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
    this->currentFunction = NONE_FUNCTION;
    this->currentClass = NONE_CLASS;
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

stmt_rv Resolver::visit(Blockvp stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
}

stmt_rv Resolver::visit(Classvp stmt) {
    ClassType enclosingClass = currentClass;
    currentClass = CLASS_;

    declare(stmt->name);
    define(stmt->name);


    if (
        stmt->superclass != nullptr &&
        (stmt->name.lexeme == stmt->superclass->name.lexeme)
    ) {
        Util::error(
            stmt->superclass->name,
            "A class can't inherit from itself."
        );
    }

    if (stmt->superclass != nullptr) {
        currentClass = SUBCLASS;
        resolve(stmt->superclass);
    }

    if (stmt->superclass != nullptr) {
      beginScope();
      (*(scopes->peek()))["super"] = true;
    }

    beginScope();
    (*(scopes->peek()))["this"] = true;

    for (auto method : stmt->methods) {
        FunctionType declaration = METHOD;
        if (method->name.lexeme == "init"){
            declaration = INITIALIZER;
        }
        resolveFunction(method, declaration); 
    }

    if (stmt->superclass != nullptr) endScope();

    endScope();
    currentClass = enclosingClass;
}

stmt_rv Resolver::visit(Varvp stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
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
    if (scopes->empty()) return nullptr;
    if (
        scopes->peek()->find(expr->name.lexeme) != scopes->peek()->end() &&
        scopes->peek()->at(expr->name.lexeme) == false
    ) {
        Util::error(expr->name,
            "Can't read local variable in its own initializer.");
    }

    resolveLocal(expr, expr->name);
    return nullptr;
}

void Resolver::resolveLocal(Exprvp expr, Token name) {
    Mapsb* currScope = scopes->peek();
    for (int i = scopes->size() - 1; i >= 0; i--) {
        if (scopes->at(i)->find(name.lexeme) != scopes->at(i)->end()) {
            interpreter->resolve(expr, scopes->size() - 1 - i);
            return;
        }
        
    }
}

rv Resolver::visit(Assignvp expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

stmt_rv Resolver::visit(Functionvp stmt){
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION);
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

stmt_rv Resolver::visit(Expressionvp stmt) {
    resolve(stmt->expression);
}

stmt_rv Resolver::visit(Ifvp stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
}

stmt_rv Resolver::visit(Printvp stmt) {
    resolve(stmt->expression);
}

stmt_rv Resolver::visit(Returnvp stmt) {
    if (currentFunction == NONE_FUNCTION) {
      Util::error(stmt->keyword, "Can't return from top-level code.");
    }

    if (stmt->value != nullptr) {
        if (currentFunction == INITIALIZER) {
            Util::error(
                stmt->keyword,
                "Can't return a value from an initializer."
            );
        }
        resolve(stmt->value);
    }

}

stmt_rv Resolver::visit(Whilevp stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
}

rv Resolver::visit(Binvp expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}


rv Resolver:: visit(Callvp expr) {
    resolve(expr->callee);

    for (auto argument : expr->arguments) {
      resolve(argument);
    }

    return nullptr;
}

rv Resolver:: visit(Groupvp expr) {
    resolve(expr->expression);
    return nullptr;
}

rv Resolver:: visit(Litvp expr) {
    return nullptr;
}

rv Resolver:: visit(Logicalvp expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

rv Resolver:: visit(Unavp expr) {
    resolve(expr->right);
    return nullptr;
}

rv Resolver::visit(Getvp expr) {
    resolve(expr->object);
    return nullptr;
}

rv Resolver::visit(Setvp expr) {
    resolve(expr->value);
    resolve(expr->object);
    return nullptr;
}

rv Resolver::visit(Supervp expr) {\
    if (currentClass == NONE_CLASS) {
      Util::error(expr->keyword,
          "Can't use 'super' outside of a class.");
    } else if (currentClass != SUBCLASS) {
      Util::error(expr->keyword,
          "Can't use 'super' in a class with no superclass.");
    }
    resolveLocal(expr, expr->keyword);
    return nullptr;
}


rv Resolver::visit(Thisvp expr) {
    if (currentClass == NONE_CLASS) {
      Util::error(expr->keyword,
          "Can't use 'this' outside of a class.");
      return nullptr;
    }
    resolveLocal(expr, expr->keyword);
    return nullptr;
}
