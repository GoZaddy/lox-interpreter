#include <iostream>
#include <fstream>
#include <sstream>

#include "scanner.h"

using namespace std;

bool hadError = false;
 
void run(string source){
    // get tokens
    Scanner scanner(source);
    vector<Token> tokens = scanner.scanTokens();

    // For now, just print the tokens.
    for (Token token : tokens) {
      cout << token.toString() << endl;
    }
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
    
    if (hadError) return 65;

    return 0;
}

void runPrompt(){
    string line = "";
    while(!cin.eof()){
        cout << "> " << endl;
        getline(cin, line);

        run(line);
        hadError = false;
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