# Chapter 7
maybe write note here

# Run instructions
```
g++ -std=c++11 main.cpp scanner.cpp token.cpp util.cpp parser.cpp interpreter.cpp
```

# What we learned
i discovered that i had to store each token in its raw form - for example, storing the string "abc" as ""abc"" instead of "abc" since our interpreter visit methods return only strings and I cannot use some arbitrary class field to differentiate, for exmample, numbers and strings