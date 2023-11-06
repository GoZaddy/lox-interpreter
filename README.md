# Chapter 6
maybe write note here

# Run instructions
```
g++ -std=c++11 main.cpp scanner.cpp token.cpp util.cpp ast_printer.cpp parser.cpp
```

# Potential issues
- In c++, I have to make use of pointers to take advantage of the visitor pattern(this is because C++ doesn't support interfaces as a distinct construct, instead I have to use an abstract class which requires pointers). Since, I have to create pointers to represent each expression, the interpreter will be creating lots of pointers which means we could theoretically run out of memory very quickly.

Haven't really thought of a solution but I think instead of returning Exprv from my parsing functions, I could processing certain expressions as soon as possible and free the pointers, returning the results of the processing instead of the actual pointers themselves
