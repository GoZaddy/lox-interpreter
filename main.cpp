#include <iostream>
#include <fstream>
#include <sstream>

#include "scanner.h"
#include "parser.cpp"
#include "ast_printer.cpp"
#include "util.h"
#include "interpreter.cpp"

using namespace std;

bool Util::hadError = false;
bool Util::hadRuntimeError = false;

Interpreter::Interpreter interpreter;
 
void run(string source){
    // get tokens
    Scanner scanner(source);
    vector<Token> tokens = scanner.scanTokens();

    // // For now, just print the tokens.
    // for (Token token : tokens) {
    //   cout << token.toString() << endl;
    // }


    Parser parser(tokens);

    // Exprvp expression = parser.parse();

    if (Util::hadError) return;


    // AstPrinter asp;
    // cout << asp.print(expression) << endl;

    // interpreter.interpret(expression);
}


int runFile(string path){
    ifstream fileStream;
    fileStream.open(path);
    if (!fileStream.is_open()){
        cout << "Could not open file: " << path << endl;
        return 74;
    }

    stringstream buffer;

    buffer << fileStream.rdbuf();

    string fileContent = buffer.str();

    run(fileContent);
    
    if (Util::hadError) return 65;
    if (Util::hadRuntimeError) return 70;

    return 0;
}

void runPrompt(){
    string line = "";
    while(!cin.eof()){
        cout << "> " << endl;
        getline(cin, line);

        run(line);
        Util::hadError = false;
    }
}


int main(int argc, char *argv[]){
    if (argc > 2){
        return 64;  // https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html
    } else if (argc == 2){
        return runFile(argv[1]);
    } else {
        runPrompt();
        
    }

    return 0;
}