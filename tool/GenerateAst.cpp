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
        processedFields.push_back(std::make_pair(currType, currFieldName));
    }

    // // helper to print out value of vector
    // for(int i = 0; i < processedFields.size(); ++i){
    //     std::pair<string, string> fieldVal = processedFields[i];
    //     cout << fieldVal.first << "------" << fieldVal.second << endl;
    // }

    
    fileStream << "class " << className << ": public " << baseName << "{" << endl;

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
    fileStream << "\t\t" << "}" << endl;

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
    fileStream << "class Visitor {" << endl;

    for (auto type : subtypes){
        int colonIndex = type.find(':');
        helperStream << type.substr(0, colonIndex);
        string typeName;
        helperStream >> typeName;

        fileStream << "\tT visit" << typeName << baseName 
            << "(" << typeName << " " << lowercase(baseName) << ") = 0;" << endl;
    }

    fileStream << "};" << endl << endl;
    


    // writer.println("  interface Visitor<R> {");

    // for (String type : types) {
    //   String typeName = type.split(":")[0].trim();
    //   writer.println("    R visit" + typeName + baseName + "(" +
    //       typeName + " " + baseName.toLowerCase() + ");");
    // }

    // writer.println("  }");
}

void defineAst(
    string outputDir, 
    string baseName, 
    std::vector<string>& subtypes
){
    string path = outputDir + "/" + baseName + ".cpp";
    ofstream fileStream;
    fileStream.open(path);
    if (!fileStream.is_open()){
        cerr << "Could not open file: " << path << endl;
        return; //74
    }

    stringstream helperStream;

    fileStream << "#include <string>" << endl;
    fileStream << "#include \"token.h\"" << endl;
    fileStream << "using namespace std;" << endl << endl;

    fileStream << "class " << baseName << " {" << endl;
    fileStream << "};" << endl << endl;

    defineVisitor(fileStream, baseName, subtypes);

    for (auto type : subtypes){
        int colonIndex = type.find(':');
        helperStream << type.substr(0, colonIndex);
        string className;
        helperStream >> className;
        string fields = type.substr(colonIndex+1, type.size()-colonIndex-1);
        defineType(fileStream, baseName, className, fields);
    }

    
    fileStream.close();
  }

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: generate_ast <output directory>" << endl;
        return 64;
    }

    string outputdir = argv[1];

    vector<string> subtypes = {
      "Binary   : Expr left, Token operatorToken, Expr right",
      "Grouping : Expr expression",
      "Literal  : string value",
      "Unary    : Token operatorToken, Expr right"
    };

    defineAst(outputdir, "Expr", subtypes);


    return 0;
}