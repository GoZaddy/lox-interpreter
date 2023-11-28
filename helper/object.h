#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

#define OUTPUT_STREAM_BODY() \
    do {                      \
        os << input.getValue(); \
        return os;            \
    } while (false)

enum Type {
    STRING,
    DOUBLE,
    BOOLEAN,
    LOX_CALLABLE,
    LOX_CLASS,
    LOX_INSTANCE,
    LOX_FUNCTION
};

class Object {
public:
    virtual Type getType() const = 0;
};

class String : public Object {
private:
    std::string value;

public:
    String(const std::string val);

    Type getType() const override;

    std::string getValue() const;

    String operator+(const String& other) const;

    String operator+(std::string& other) const;

    bool operator==(const String& other) const;

    friend std::ostream& operator<<(std::ostream& os, const String& str);
};

class Double : public Object {
private:
    double value;

public:
    Double(double val);

    Type getType() const override;

    double getValue() const;

    Double operator*(const Double& other) const;

    Double operator+(const Double& other) const;

    Double operator-(const Double& other) const;

    Double operator/(const Double& other) const;

    bool operator==(const Double& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Double& str);
};

class Boolean : public Object {
private:
    bool value;

public:
    Boolean(bool val);

    Type getType() const override;

    bool getValue() const;

    Boolean operator!() const;

    bool operator==(const Boolean& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Boolean& str);
};

Object* True = new Boolean(true);
Object* False = new Boolean(false);

#endif // OBJECT_H