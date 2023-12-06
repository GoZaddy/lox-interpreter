// #include "interpreter.h"
// #include "lox_class.cpp"

#include "declr.h"

#define GET_DOUBLE(x) Util::GetDouble(x)

#define GET_STRING(x) Util::GetString(x)


rv Interpreter::evaluate(Exprvp expr){
    rv value = expr->accept(this);
    // delete expr; // TODO: test this out
    return value;
}


void Interpreter::checkNumberOperand(Token operatorToken, rv operand){
    if (Util::isNumberLiteral(operand)) return;
    throw Util::runtimeError(operatorToken, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operatorToken, rv left, rv right){
    if (Util::isNumberLiteral(left) && Util::isNumberLiteral(right)) return;
    throw Util::runtimeError(operatorToken, "Operands must be numbers");
}


void Interpreter::execute(Stmtvp stmt){
    stmt->accept(this);
}

void Interpreter::resolve(Exprvp expr, int depth) {
    locals[expr] = depth;
}

Interpreter::Interpreter() {
    globals = new Environment();
    environment = globals;
    LoxCallable* clockFunc = new Clock();
    globals->define("clock", clockFunc); // TODO: figure this out, consider using a new map in Environment for just functions
}
void Interpreter::interpret(std::vector<Stmtvp> statements){
    try{
        for (int i = 0; i < statements.size(); ++i){
            execute(statements[i]);
            // delete statements[i]; // i was testing something
        }
    } catch(string error){std::cerr << error << endl;};
}

void Interpreter::executeBlock(
    std::vector<Stmtvp> statements,
    Environment* currentEnv
) {
    Environment* previous = this->environment;
    try {
        this->environment = currentEnv;

        for (auto statement : statements) {
            execute(statement);
        }
    } 
    catch(string error){
        std::cerr << error << endl;
    }
    catch(ReturnException error){
        this->environment = previous;
        throw error;
    } catch(...){}
    
    this->environment = previous;
    
}


rv Interpreter::visit(Litv* expr){
    return expr->value;
}

rv Interpreter::visit(Logicalvp expr) {
    rv left = evaluate(expr->left);

    if (expr->operatorToken.type == OR) {
        if (Util::isTruthy(left)) return left;
    } else {
        if (!Util::isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

rv Interpreter::visit(Setvp expr) {
    
    rv object = evaluate(expr->object);

    if (!Util::isInstance(object)) { 
        throw Util::runtimeError(
            expr->name,
            "Only instances have fields."
        );
    }

    rv value = evaluate(expr->value);
    ((LoxInstance*) object)->set(expr->name, value);
    return value;
  }

rv Interpreter::visit(Groupv* expr){
    return evaluate(expr->expression);
}

rv Interpreter::visit(Unav* expr){
    rv right = evaluate(expr->right);

    switch(expr->operatorToken.type){
        case MINUS:
            checkNumberOperand(expr->operatorToken, right);
            return new Double(-1 * ((Double*) right)->getValue());
        case BANG:
            return Util::isTruthy(right) ? False : True;
        default:
            //unreachable
            return nullptr; // empty string is null for us
    }
}

rv Interpreter::visit(Binv* expr){
    rv left = evaluate(expr->left);
    rv right = evaluate(expr->right);

    switch(expr->operatorToken.type){
        case MINUS:
            checkNumberOperands(expr->operatorToken, left, right);
            return new Double(GET_DOUBLE(left) - GET_DOUBLE(right));
        case SLASH:
            checkNumberOperands(expr->operatorToken, left, right);
            return new Double(GET_DOUBLE(left) / GET_DOUBLE(right));
        case STAR:
            checkNumberOperands(expr->operatorToken, left, right);
            return new Double(GET_DOUBLE(left) * GET_DOUBLE(right));
        case PLUS:
            if (Util::isStringLiteral(left) && Util::isStringLiteral(right)){
                return new String(GET_STRING(left) + GET_STRING(right));
            }
            
            if (Util::isNumberLiteral(left) && Util::isNumberLiteral(right)){
                return new Double(GET_DOUBLE(left) + GET_DOUBLE(right));
            }

            throw Util::runtimeError(expr->operatorToken, "Operands must be two numbers or two strings.");
            
        case GREATER:
            checkNumberOperands(expr->operatorToken, left, right);
            return (GET_DOUBLE(left) > GET_DOUBLE(right)) ? True : False;
        case GREATER_EQUAL:
            checkNumberOperands(expr->operatorToken, left, right);
            return (GET_DOUBLE(left) >= GET_DOUBLE(right)) ? True : False;
        case LESS:
            checkNumberOperands(expr->operatorToken, left, right);
            return (GET_DOUBLE(left) < GET_DOUBLE(right)) ? True : False;
        case LESS_EQUAL:
            checkNumberOperands(expr->operatorToken, left, right);
            return (GET_DOUBLE(left) <= GET_DOUBLE(right)) ? True : False;
            
        case BANG_EQUAL:
            return Util::isEqual(left, right) ? False : True;
        case EQUAL_EQUAL:
            return Util::isEqual(left, right) ? True : False;
            

        default:
            //unreachable
            return nullptr;
    }
}

rv Interpreter::visit(Callvp expr) {
    rv callee = evaluate(expr->callee);
   

    if (!Util::isCallable(callee) && !Util::isClass(callee)){
        throw Util::runtimeError(expr->paren, "Can only call functions and classes.");
    }


    LoxCallable* func = (LoxCallable*) callee;

    

    std::vector<rv> arguments;
    for (auto argument : expr->arguments) { 
        arguments.push_back(evaluate(argument));
    }


    if (arguments.size() != func->arity()) {
        throw Util::runtimeError(expr->paren, "Expected " +
            std::to_string(func->arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");
    }
    return func->call(this, arguments);
}

rv Interpreter::visit(Getvp expr) {
    rv object = evaluate(expr->object);
    if (Util::isInstance(object)) {
        return ((LoxInstance*) object)->get(expr->name);
    }

    throw Util::runtimeError(expr->name,
        "Only instances have properties.");
}


rv Interpreter::visit(Variablevp expr){
    return lookUpVariable(expr->name, expr);
}

rv Interpreter::lookUpVariable(Token name, Exprvp expr) {
    if (locals.find(expr) != locals.end()) {
        int distance = locals[expr];
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}

rv Interpreter::visit(Assignvp expr){
    rv value = evaluate(expr->value);
    
    if (locals.find(expr) != locals.end()) {
        int distance = locals[expr];
        environment->assignAt(distance, expr->name, value);
    } else {
        globals->assign(expr->name, value);
    }
    return value;
}

stmt_rv Interpreter::visit(Expressionvp stmt){
    evaluate(stmt->expression);
}

stmt_rv Interpreter::visit(Functionvp stmt){
    LoxCallable* func = new LoxFunction(stmt, environment, false);
    environment->define(stmt->name.lexeme, func); // come up with our solution for this
}

stmt_rv Interpreter::visit(Ifvp stmt) {
    if (Util::isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
}

stmt_rv Interpreter::visit(Printvp stmt){
    rv value = evaluate(stmt->expression);
    switch (value->getType()){
        case LOX_CLASS:
            std::cout << ((LoxClass*) value)->toString() << endl;
            break;
        case LOX_INSTANCE:
            std::cout << ((LoxInstance*) value)->toString() << endl;
            break;
        case BOOLEAN:
            std::cout << (((Boolean*) value)->getValue() ? "true" : "false") << endl;
            break;
        case STRING_T:
            std::cout << ((String*) value)->getValue() << endl;
            break;
        case DOUBLE_T:
            std::cout << ((Double*) value)->getValue() << endl;
            break;
        default:
            std::cout << value << endl;
            break;
    }
}

stmt_rv Interpreter::visit(Returnvp stmt) {
    rv value = nullptr;
    if (stmt->value != nullptr) value = evaluate(stmt->value);

    throw ReturnException(value);
}

stmt_rv Interpreter::visit(Varvp stmt){
    rv value = nullptr;
    if (stmt->initializer != nullptr){
        value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, value);
}

stmt_rv Interpreter::visit(Whilevp stmt) {
    while (Util::isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
}

stmt_rv Interpreter::visit(Blockvp stmt) {
    Environment* newEnv = new Environment(environment); 
    executeBlock(stmt->statements, newEnv);
}

stmt_rv Interpreter::visit(Classvp stmt) {
    rv superclass = nullptr;
    if (stmt->superclass != nullptr) {
        superclass = evaluate(stmt->superclass);
        if (!Util::isClass(superclass)) {
            throw Util::runtimeError(
                stmt->superclass->name,
                "Superclass must be a class."
            );
        }
    }

    environment->define(stmt->name.lexeme, nullptr);

    if (stmt->superclass != nullptr) {
        environment = new Environment(environment);
        environment->define("super", superclass);
    }

    std::unordered_map<std::string, LoxFunction*> methods;
    for (auto method : stmt->methods) {
        LoxFunction* function = new LoxFunction(method, environment, method->name.lexeme == "init");
        methods[method->name.lexeme] =  function;
    }

    LoxClass* klass = new LoxClass(stmt->name.lexeme, ((LoxClass*) superclass), methods);

    if (superclass != nullptr) {
      environment = environment->enclosing;
    }

    environment->define(stmt->name.lexeme, klass); 
}

rv Interpreter::visit(Thisvp expr) {
    return lookUpVariable(expr->keyword, expr);
}


rv Interpreter::visit(Supervp expr) {
    int distance = locals[expr];
    LoxClass* superclass = (LoxClass*) environment->getAt(
        distance, "super");

    LoxInstance* object = (LoxInstance*) environment->getAt(
        distance - 1, "this");

    LoxFunction* method = superclass->findMethod(expr->method.lexeme);
    if (method == nullptr) {
      throw Util::runtimeError(expr->method,
          "Undefined property '" + expr->method.lexeme + "'.");
    }

    return method->bind(object);
}