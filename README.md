# Chapter 11

I had to get a bit creative with binding LoxInstances to methods because my visitor functions return strings unlike the book which returns Java Objects. To accomplish this, i returned a string key that contains 3 keys - a key to get a class definition, a key to get a class method and a key to get an instance. With these keys, I was able to then call the bind method in Interpreter::visit(Callvp expr) if the callee is known to be a class method


# Run instructions
```
g++ -std=c++11 main.cpp scanner.cpp token.cpp util.cpp parser.cpp expr.cpp stmt.cpp interpreter.cpp return.cpp lox_function.cpp native_funcs.cpp environment.cpp resolver.cpp lox_class.cpp lox_instance.cpp
```

To run AST (Abstract Syntax Tool) generator tool:
```
g++ -std=c++11 tool/GenerateAst.cpp
```

# What we learned
