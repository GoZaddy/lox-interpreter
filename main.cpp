#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool hadError = false;
 
void run(string code){
    // get tokens
}

void error(int line, string message){
    report(line, "", message);
}

void report(int line, string where, string message){
    cerr << "[line " << line << "] Error" << where << ": " << message;
}

int runFile(string path){
    ifstream fileStream;
    fileStream.open(path);
    if (!fileStream.is_open()){
        cout << "Could not open file: " << path << endl;
        return;
    }

    stringstream buffer;
    buffer << fileStream.rdbuf();

    run(buffer.str());
    if (hadError) return 65;
}

int runPrompt(){
    string line;
    while(!cin.eof()){
        cout << "> " << endl;
        getline(cin, line);

        run(line);
        hadError = false;
    }
}


int main(int argc, char *argv[]){
    if (argc > 1){
        return 64;  // https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html
    } else if (argc == 1){
        return runFile(argv[0]);
    } else {
        return runPrompt();
    }

    return 0;
}