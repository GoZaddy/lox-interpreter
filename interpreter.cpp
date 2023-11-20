#include "interpreter.h"

rv Interpreter::evaluate(Exprvp expr){
    rv value = expr->accept(this);
    // delete expr; // TODO: test this out
    return value;
}
bool Interpreter::isTruthy(rv object){
    if (object == "nil") return false; // TODO: what other values could return false
    if (object == "true") return true;
    if (object == "false") return false;
    return true;
}

bool Interpreter::isStringLiteral(string literal){
    return literal[0] == '\"' && literal[literal.size()-1] == '\"';
}

bool Interpreter::isNumberLiteral(string literal){
    char* end = nullptr;
    double val = strtod(literal.c_str(), &end);
    return end != literal.c_str() && *end == '\0';
}

bool Interpreter::isCallable(string expr){
    if (expr.size() > 2){
        if (expr[0] == '(' && expr[1] == ')'){
            return true;
        }
    }
    return false;
}

bool Interpreter::isEqual(rv a, rv b){
    return a == b;
}

void Interpreter::checkNumberOperand(Token operatorToken, rv operand){
    if (isNumberLiteral(operand)) return;
    throw Util::runtimeError(operatorToken, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operatorToken, rv left, rv right){
    if (isNumberLiteral(left) && isNumberLiteral(right)) return;
    throw Util::runtimeError(operatorToken, "Operands must be numbers");
}

string Interpreter::stripTrailingZeroes(rv text){
    if (text.size() > 7 && text.substr(text.size()-7) == ".000000"){
        text = text.substr(0, text.size()-7);
    }
    return text;
}

void Interpreter::execute(Stmtvp stmt){
    stmt->accept(this);
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
        this->environment = previous;
    }
    catch(ReturnException error){
        this->environment = previous;
        throw error;
    } catch(...){this->environment = previous;}
    
    
    
}


rv Interpreter::visit(Litv* expr){
    return expr->value;
}

rv Interpreter::visit(Logicalvp expr) {
    rv left = evaluate(expr->left);

    if (expr->operatorToken.type == OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

rv Interpreter::visit(Groupv* expr){
    return evaluate(expr->expression);
}

rv Interpreter::visit(Unav* expr){
    rv right = evaluate(expr->right);

    switch(expr->operatorToken.type){
        case MINUS:
            checkNumberOperand(expr->operatorToken, right);
            return "-"+right;
        case BANG:
            return isTruthy(right) ? "false" : "true";
        default:
            //unreachable
            return ""; // empty string is null for us
    }
}

rv Interpreter::visit(Binv* expr){
    rv left = evaluate(expr->left);
    rv right = evaluate(expr->right);

    switch(expr->operatorToken.type){
        case MINUS:
            checkNumberOperands(expr->operatorToken, left, right);
            return stripTrailingZeroes(std::to_string(Util::doub(left) - Util::doub(right)));
        case SLASH:
            checkNumberOperands(expr->operatorToken, left, right);
            return stripTrailingZeroes(std::to_string(Util::doub(left) / Util::doub(right)));
        case STAR:
            checkNumberOperands(expr->operatorToken, left, right);
            return stripTrailingZeroes(std::to_string(Util::doub(left) * Util::doub(right)));
        case PLUS:
            if (isStringLiteral(left) && isStringLiteral(right)){
                left.pop_back();
                return left + right.substr(1);
            }
            
            if (isNumberLiteral(left) && isNumberLiteral(right)){
                return stripTrailingZeroes(std::to_string(Util::doub(left) + Util::doub(right)));
            }

            throw Util::runtimeError(expr->operatorToken, "Operands must be two numbers or two strings.");
            
        case GREATER:
            checkNumberOperands(expr->operatorToken, left, right);
            return (Util::doub(left) > Util::doub(right)) ? "true" : "false";
        case GREATER_EQUAL:
            checkNumberOperands(expr->operatorToken, left, right);
            return (Util::doub(left) >= Util::doub(right)) ? "true" : "false";
        case LESS:
            checkNumberOperands(expr->operatorToken, left, right);
            return (Util::doub(left) < Util::doub(right)) ? "true" : "false";
        case LESS_EQUAL:
            checkNumberOperands(expr->operatorToken, left, right);
            return (Util::doub(left) <= Util::doub(right)) ? "true" : "false";
            
        case BANG_EQUAL:
            return isEqual(left, right) ? "false" : "true";
        case EQUAL_EQUAL:
            return isEqual(left, right) ? "true" : "false";
            

        default:
            //unreachable
            return "";
    }
}

rv Interpreter::visit(Callvp expr) {
    rv callee = evaluate(expr->callee);

    if (!isCallable(callee)){
        throw Util::runtimeError(expr->paren, "Can only call functions and classes.");
    }

    std::vector<rv> arguments;
    for (auto argument : expr->arguments) { 
        // cout << "argument " << evaluate(argument) << endl;
        arguments.push_back(evaluate(argument));
    }


    LoxCallable* func = environment->getCallable(callee); // figure this out

    if (arguments.size() != func->arity()) {
        throw Util::runtimeError(expr->paren, "Expected " +
            std::to_string(func->arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");
    }
    return func->call(this, arguments);
}

rv Interpreter::visit(Variablevp expr){
    return environment->get(expr->name);
}

rv Interpreter::visit(Assignvp expr){
    rv value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return value;
}

rv Interpreter::visit(Expressionvp stmt){
    return evaluate(stmt->expression);
}

rv Interpreter::visit(Functionvp stmt){
    LoxCallable* func = new LoxFunction(stmt, environment);
    environment->defineFunc(stmt->name.lexeme, func); // come up with our solution for this
    return "";
}

rv Interpreter::visit(Ifvp stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
    return "";
}

rv Interpreter::visit(Printvp stmt){
    rv value = evaluate(stmt->expression);
    std::cout << value << endl;
    return "";
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
    while (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
    return "";
}

rv Interpreter::visit(Blockvp stmt) {
    Environment* newEnv = new Environment(environment); 
    executeBlock(stmt->statements, newEnv);
    return "";
}