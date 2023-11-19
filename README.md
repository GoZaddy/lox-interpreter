# Chapter 8
I had to use the visitor pattern to get a "type" for each expression subclass since c++ doesn't support an non-tedious equivalent of java's `instanceof`
# Run instructions
```
g++ -std=c++11 main.cpp scanner.cpp token.cpp util.cpp parser.cpp interpreter.cpp
```

To run AST (Abstract Syntax Tool) generator tool:
```
g++ -std=c++11 tool/GenerateAst.cpp
```

# What we learned
