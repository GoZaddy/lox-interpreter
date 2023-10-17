#include <string>
#include <vector>

#include "token.h"

using namespace std;


class Scanner {
    private:
        string source;
        vector<string> tokens;
        int start = 0;
        int current = 0;
        int line = 1;

    public:
        Scanner(string source){
            this->source = source;
        }

        vector<Token> scanTokens() {
            while (!isAtEnd()) {
            // We are at the beginning of the next lexeme.
            start = current;
            scanToken();
            }

            tokens.push_back(Token(EOF, "", null, line));
            return tokens;
        }
};