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
g++ -w -std=c++11 main.cpp expr.cpp stmt.cpp scanner.cpp token.cpp util.cpp parser.cpp interpreter.cpp
```

*Note: Compilation can be unusually slow(~10 seconds)*
