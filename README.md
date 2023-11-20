# Chapter 10

Since c++ doesn't support an catch-all type (Java's equivalent of the Object type), I decided to represent all expressions as strings early on in the project. Language keywords were represented in plain strings like "nil", "false", "true" while strings themselves simply retained their quotes. This way I could tell a string ""123"" apart from a number "123". The Environment class in the book contains a map from strings to Objects which my code implemented as a map of strings to strings. In chapter 10, the book adds supports for functions and in order to keep track of defined functions in scope, function names(strings) are mapped to LoxCallable objects. Unlike a string or number, it's hard to encode a LoxCallable as a string so my solution was to simply implement a rule(in the literal sense):
    for every function name, "func", map "func" to "()func" and then, in another map that maps strings to LoxCallables, map "()func" to the corresponding LoxCallable.

This way, we ensure we can easily verify the existence of certain names in the scope and retrieve the respective LoxCallable as required.

# Run instructions
```
g++ -std=c++11 main.cpp scanner.cpp token.cpp util.cpp parser.cpp 
expr.cpp stmt.cpp interpreter.cpp return.cpp lox_function.cpp native_funcs.cpp environment.cpp
```

To run AST (Abstract Syntax Tool) generator tool:
```
g++ -std=c++11 tool/GenerateAst.cpp
```

# What we learned
