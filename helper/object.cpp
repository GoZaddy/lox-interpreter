#include <iostream>
#include <string>

using namespace std;

#define OUTPUT_STREAM_BODY() \
    do {    \
        os << input.value;    \
        return os;  \
    } while(false) 

enum Type{
    STRING,
    DOUBLE,
    BOOLEAN,
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
    String(const std::string val) : value(val) {}

    Type getType() const override {
        return STRING;
    }

    std::string getValue() const {
        return value;
    }

    String operator +(const String& other) const{
        return String(this->value+other.getValue());
    }

    String operator +(std::string& other) const{
        return String(this->value+other);
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str);
};

std::ostream& operator<<(std::ostream& os, const String& input) {
    OUTPUT_STREAM_BODY();
}








class Double : public Object {
private:
    double value;

public:
    Double(double val) : value(val) {}

    Type getType() const override {
        return DOUBLE;
    }

    double getValue() const {
        return value;
    }

    Double operator*(const Double& other) const {
        return Double(value * other.getValue());
    }

    Double operator+(const Double& other) const {
        return Double(value + other.getValue());
    }

    Double operator-(const Double& other) const {
        return Double(value - other.getValue());
    }

    Double operator/(const Double& other) const {
        return Double(value / other.getValue());
    }

    friend std::ostream& operator<<(std::ostream& os, const Double& str);

};

std::ostream& operator<<(std::ostream& os, const Double& input) {
    OUTPUT_STREAM_BODY();
}

class Boolean : public Object {
private:
    bool value;

public:
    Boolean(bool val) : value(val) {}

    Type getType() const override {
        return BOOLEAN;
    }

    bool getValue() const {
        return value;
    }

    Boolean operator!() const{
        return Boolean(!value);
    }

    std::ostream& operator<<(std::ostream& os) const{
        os << value;
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const Boolean& str);
};

std::ostream& operator<<(std::ostream& os, const Boolean& input) {
    OUTPUT_STREAM_BODY();
}




int main(){
    Object* str;

    String hi("heyy");

    str = &hi;

    std::cout << "str: " << hi << endl;
    std::cout << (hi + String("faruq")) << endl;

    std::cout << (str->getType() == STRING ? "true" : "false") << endl;

    return 0;
}