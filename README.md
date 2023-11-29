# Lox Interpreter

##  Building executable for interpreter

- Make sure you're in the root directory of the project
- Make sure you have g++ compiler installed

If you're not sure:
Run `g++ --version` in command line to find out. You should get something like this:
```
g++.exe (x86_64-posix-seh, Built by strawberryperl.com project) 8.3.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### Linux/Mac
- First make the shell script executable
```
chmod +x compile.sh
```
- Run the script to compile the source code
```
./compile.sh
```

### Windows
- Run in the terminal:
```
g++ -std=c++11 helper/object.cpp main.cpp scanner.cpp token.cpp util.cpp parser.cpp expr.cpp stmt.cpp interpreter.cpp return.cpp lox_function.cpp native_funcs.cpp environment.cpp resolver.cpp lox_class.cpp lox_instance.cpp lox_callable.cpp

```


## Testing

In the tests/ directory, you will find a variety of tests to test the different parts of the interpreter. Each test has been verified to pass but you can run them to confirm the results. To test a sample lox file named inheritance.lox:

For windows:
```
./a.exe tests/inheritance.lox

```


For mac/linux:
```
./a.out tests/inheritance.lox

```

## REPL

You can also spin up a lox REPL session by running:

(Windows)
```
a.exe
```

(Mac/Linux)
```
a.out
```


