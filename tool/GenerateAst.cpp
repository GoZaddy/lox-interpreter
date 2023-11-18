#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;


string lowercase(string input){
    string result;
    for(int i = 0; i < input.size(); ++i){
        result += tolower(input[i]);
    }

    return result;
}

string uppercase(string input){
    string result;
    for(int i = 0; i < input.size(); ++i){
        result += toupper(input[i]);
    }

    return result;
}


void defineType(
    ofstream& fileStream, 
    string baseName, 
    string className, 
    string fields
){

    // preprocess fields
    stringstream ss(fields);
    string currLine;
    string currType;
    string currFieldName;

    vector<pair<string, string>> processedFields;

    stringstream helper;

    
    while(getline(ss, currLine, ',')){
        helper << currLine;
        helper >> currType >> currFieldName;
        helper.clear();
        // TODO: removing this and moving the <T>* type prefix to the user input instead
        // if (currType == baseName){
        //     currType += "<T>*";
        // }
        processedFields.push_back(std::make_pair(currType, currFieldName));
    }

    // // helper to print out value of vector
    // for(int i = 0; i < processedFields.size(); ++i){
    //     std::pair<string, string> fieldVal = processedFields[i];
    //     cout << fieldVal.first << "------" << fieldVal.second << endl;
    // }

    fileStream << "template <typename T>" << endl;
    fileStream << "class " << className << ": public " << baseName << "<T> {" << endl;

    fileStream << "\tpublic:" << endl;


    // instance variables
    for (auto fieldVal: processedFields){
        fileStream << "\t\t" << fieldVal.first << " " << fieldVal.second << ";" << endl;
    }


    string constructorArgs;
    for (int i = 0; i < processedFields.size(); ++i){
        std::pair<string, string> fieldVal = processedFields[i];
        constructorArgs += fieldVal.first + " " + fieldVal.second;

        if (i != processedFields.size()-1){
            constructorArgs += ", ";
        }
    }
    
    // constructor
    fileStream << "\t\t" << className << "(" << constructorArgs << ") {" << endl;
    for (int i = 0; i < processedFields.size(); ++i){
        std::pair<string, string> fieldVal = processedFields[i];
        fileStream << "\t\t\t" << "this->" << fieldVal.second << " = " << fieldVal.second << ";" << endl;
    }
    fileStream << "\t\t}" << endl;

    // overriding accept() method
    fileStream << endl;
    fileStream << "\t\tT accept(" << baseName << "Visitor<T>* visitor) {" << endl;
    fileStream << "\t\t\treturn visitor->visit" << "(this);" << endl;
    fileStream << "\t\t}" << endl;



    fileStream << "};" << endl << endl;

}

void defineVisitor(
    ofstream& fileStream, 
    string baseName, 
    vector<string>& subtypes
){
    stringstream helperStream;

    fileStream << "// visitor interface" << endl;
    fileStream << "template <typename T>" << endl;
    fileStream << "class " << baseName << "Visitor {" << endl;
    fileStream << "\tpublic:" << endl;

    for (auto type : subtypes){
        int colonIndex = type.find(':');
        helperStream << type.substr(0, colonIndex);
        string typeName;
        helperStream >> typeName;

        fileStream << "\t\tvirtual T visit"
            << "(" << typeName << "<T>* " << lowercase(baseName) << ") = 0;" << endl;
    }

    fileStream << "};" << endl << endl;
}

void defineAst(
    string outputDir, 
    string baseName, 
    std::vector<string>& subtypes,
    string beginningStatements = ""
){
    string path = outputDir + "/" + lowercase(baseName) + ".cpp";
    ofstream fileStream;
    fileStream.open(path);
    if (!fileStream.is_open()){
        cerr << "Could not open file: " << path << endl;
        return; //74
    }

    stringstream helperStream;
    
    fileStream << "#ifndef " << uppercase(baseName) << endl;
    fileStream << "#define " << uppercase(baseName) << endl;

    fileStream << "#include <string>" << endl;
    fileStream << "#include \"token.h\"" << endl;
    fileStream << beginningStatements << endl;
    fileStream << "using namespace std;" << endl << endl;

    // forward declaration to deal with circular dependencies
    fileStream << "template <typename T>" << endl;
    fileStream << "class " << baseName << ";" << endl << endl;

    fileStream << "template <typename T>" << endl;
    fileStream << "class " << baseName << "Visitor;" << endl << endl << endl;



    for (auto type : subtypes){
        int colonIndex = type.find(':');
        helperStream << type.substr(0, colonIndex);
        string className;
        helperStream >> className;
        string fields = type.substr(colonIndex+1, type.size()-colonIndex-1);
        defineType(fileStream, baseName, className, fields);
    }

    defineVisitor(fileStream, baseName, subtypes);

    fileStream << "template <typename T>" << endl;
    fileStream << "class " << baseName << " {" << endl;
    fileStream << "\tpublic:" << endl;
    fileStream << "\t\tvirtual T accept(" << baseName << "Visitor<T>* visitor) = 0;" << endl;
    fileStream << "};" << endl << endl;

    fileStream << "#endif" << endl;
    fileStream.close();
  }

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: generate_ast <output directory>" << endl;
        return 64;
    }

    string outputdir = argv[1];

    vector<string> expressionSubtypes = {
      "Binary   : Expr<T>* left, Token operatorToken, Expr<T>* right",
      "Grouping : Expr<T>* expression",
      "Literal  : string value",
      "Unary    : Token operatorToken, Expr<T>* right",
      "Variable : Token name"
    };

    defineAst(outputdir, "Expr", expressionSubtypes);


    vector<string> statementSubtypes = {
        "Expression : Expr<T>* expression",
        "Print      : Expr<T>* expression",
        "Var        : Token name, Expr<T>* initializer"
    };

    defineAst(outputdir, "Stmt", statementSubtypes, "#include \"expr.cpp\"\n");


    // statements



    return 0;
}