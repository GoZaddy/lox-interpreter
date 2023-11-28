// #include "interpreter.h"
// #include "lox_class.cpp"

#include "declr.h"
#include <sstream>


#define GET_DOUBLE(x) Util::GetDouble(x)

#define GET_STRING(x) Util::GetString(x)


// Looking back, I probably could have created helper classes similar to Stmt and Expr or even somehow
// used both those classes to represent the return type of these evaluate() expressions
// i.e create a pure virtual class that expr and stmt derive from and use a pointer
// to that virtual class as the return type for the evaluate function. We can then cast that type as we 
// need to more specific types

rv Interpreter::evaluate(Exprvp expr){
    rv value = expr->accept(this);
    // delete expr; // TODO: test this out
    return value;
}

// bool Interpreter::Util::isStringLiteral(string literal){
//     return literal[0] == '\"' && literal[literal.size()-1] == '\"';
// }

// bool Interpreter::Util::isNumberLiteral(string literal){
//     char* end = nullptr;
//     double val = strtod(literal.c_str(), &end);
//     return end != literal.c_str() && *end == '\0';
// }

// bool Interpreter::Util::isCallable(string expr){
//     if (expr.size() > 2){
//         if (expr.substr(0,2) == "()" || (expr.substr(0,3) == "(.)")){
//             return true;
//         }
//     }
//     return false;
// }


// bool Interpreter::isClassMethod(string expr){
//     return expr.substr(0,3) == "(.)";
// }

// bool Interpreter::Util::isClass(string expr){
//     if (expr.size() > 7){
//         if (expr.substr(0,7) == "(class)"){
//             return true;
//         }
//     }
//     return false;
// }

// bool Interpreter::Util::isInstance(string expr){
//     if (expr.size() > 10){
//         if (expr.substr(0,10) == "(instance)"){
//             return true;
//         }
//     }
//     return false;
// }

// bool Interpreter::isEqual(rv a, rv b){
//     return a == b;
// }

void Interpreter::checkNumberOperand(Token operatorToken, rv operand){
    if (Util::isNumberLiteral(operand)) return;
    throw Util::runtimeError(operatorToken, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operatorToken, rv left, rv right){
    if (Util::isNumberLiteral(left) && Util::isNumberLiteral(right)) return;
    throw Util::runtimeError(operatorToken, "Operands must be numbers");
}

// string Interpreter::stripTrailingZeroes(rv text){
//     if (text.size() > 7 && text.substr(text.size()-7) == ".000000"){
//         text = text.substr(0, text.size()-7);
//     }
//     return text;
// }

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
    globals->defineFunc("clock", clockFunc); // TODO: figure this out, consider using a new map in Environment for just functions
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
    (environment->getInstance(object))->set(expr->name, value);
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

    

    std::vector<rv> arguments;
    for (auto argument : expr->arguments) { 
        arguments.push_back(evaluate(argument));
    }

    LoxCallable* func;


    if (Util::isCallable(callee)){
        if (isClassMethod(callee)){
            // todo: figure out a way to get instance key here
            // we can just call bind here 
            stringstream ss(callee);
            string className, methodName, instanceKey;
            ss >> className >> className >> methodName >> instanceKey;

            
            func = environment->getClassMethod(callee);
            func = ((LoxFunction*) func)->bind(environment->getInstance(instanceKey));
        } else {
            func = environment->getCallable(callee);
        }
    } else {
        // is class
        func = environment->getClass(callee);
    }


    if (arguments.size() != func->arity()) {
        throw Util::runtimeError(expr->paren, "Expected " +
            (func->arity()) + " arguments but got " +
            (arguments.size()) + ".");
    }
    return func->call(this, arguments);
}

rv Interpreter::visit(Getvp expr) {
    rv object = evaluate(expr->object);
    if (Util::isInstance(object)) {
        return environment->getInstance(object)->get(expr->name);
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

rv Interpreter::visit(Expressionvp stmt){
    return evaluate(stmt->expression);
}

rv Interpreter::visit(Functionvp stmt){
    // TODO: create a combination key for functions st
    LoxCallable* func = new LoxFunction(stmt, environment, false);
    environment->defineFunc(stmt->name.lexeme, func); // come up with our solution for this
    return nullptr;
}

rv Interpreter::visit(Ifvp stmt) {
    if (Util::isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
    return nullptr;
}

rv Interpreter::visit(Printvp stmt){
    rv value = evaluate(stmt->expression);
    if (Util::isClass(value)){
        std::cout << environment->getClass(value)->toString() << endl;
        return null;
    } else if (Util::isInstance(value)){
        std::cout << environment->getInstance(value)->toString() << endl;
        return null;
    } else if (Util::isStringLiteral(value)){
        std::cout << value.substr(1,value.size()-2) << endl; //trim quotes
        return null;
    }
    std::cout << value << endl;
    return null;
}

rv Interpreter::visit(Returnvp stmt) {
    rv value = "nil";
    if (stmt->value != nullptr) value = evaluate(stmt->value);

    throw ReturnException(value);
}

rv Interpreter::visit(Varvp stmt){
    string value = "";
    if (stmt->initializer != nullptr){
        value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, value);
    return value;
}

rv Interpreter::visit(Whilevp stmt) {
    while (Util::isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
    return nullptr;
}

rv Interpreter::visit(Blockvp stmt) {
    Environment* newEnv = new Environment(environment); 
    executeBlock(stmt->statements, newEnv);
    return nullptr;
}

rv Interpreter::visit(Classvp stmt) {
    rv superclass = null;
    if (stmt->superclass != nullptr) {
        superclass = evaluate(stmt->superclass);
        if (!Util::isClass(superclass)) {
            throw Util::runtimeError(
                stmt->superclass->name,
                "Superclass must be a class."
            );
        }
    }

    environment->define(stmt->name.lexeme, null);

    if (stmt->superclass != nullptr) {
        environment = new Environment(environment);
        environment->define("super", superclass);
    }

    std::unordered_map<std::string, LoxFunction*> methods;
    for (auto method : stmt->methods) {
        LoxFunction* function = new LoxFunction(method, environment, method->name.lexeme == "init");
        methods[method->name.lexeme] =  function;
    }

    LoxClass* superclassPtr = superclass == null ? nullptr : environment->getClass(superclass);

    LoxClass* klass = new LoxClass(stmt->name.lexeme, superclassPtr, methods);

    if (superclass != null) {
      environment = environment->enclosing;
    }

    environment->addClass(stmt->name.lexeme, klass); 
    return null;
}

rv Interpreter::visit(Thisvp expr) {
    return lookUpVariable(expr->keyword, expr);
}


rv Interpreter::visit(Supervp expr) {
    int distance = locals[expr];
    LoxClass* superclass = environment->getClass(environment->getAt(
        distance, "super"));

    LoxInstance* object = environment->getInstance(environment->getAt(
        distance - 1, "this"));

    rv method_key = superclass->findMethod(expr->method.lexeme);
    if (method_key == null) {
      throw Util::runtimeError(expr->method,
          "Undefined property '" + expr->method.lexeme + "'.");
    }

    stringstream ss;
    ss << method_key << " (instance)" << object; 
    // return key containing class key, method key and instance key
    return ss.str();
}