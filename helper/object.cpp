#include "object.h"

String::String(const std::string val) : value(val) {}

Type String::getType() const {
    return STRING_T;
}

std::string String::getValue() const {
    return value;
}

String String::operator+(const String& other) const {
    return String(this->value + other.getValue());
}

String String::operator+(std::string& other) const {
    return String(this->value + other);
}

bool String::operator==(const String& other) const {
    return this->value == other.getValue();
}

std::ostream& operator<<(std::ostream& os, const String& input) {
    OUTPUT_STREAM_BODY();
}

Double::Double(double val) : value(val) {}

Type Double::getType() const {
    return DOUBLE_T;
}

double Double::getValue() const {
    return value;
}

Double Double::operator*(const Double& other) const {
    return Double(value * other.getValue());
}

Double Double::operator+(const Double& other) const {
    return Double(value + other.getValue());
}

Double Double::operator-(const Double& other) const {
    return Double(value - other.getValue());
}

Double Double::operator/(const Double& other) const {
    return Double(value / other.getValue());
}

bool Double::operator==(const Double& other) const {
    return this->value == other.getValue();
}

std::ostream& operator<<(std::ostream& os, const Double& input) {
    OUTPUT_STREAM_BODY();
}

Boolean::Boolean(bool val) : value(val) {}

Type Boolean::getType() const {
    return BOOLEAN;
}

bool Boolean::getValue() const {
    return value;
}

Boolean Boolean::operator!() const {
    return Boolean(!value);
}

bool Boolean::operator==(const Boolean& other) const {
    return this->value == other.getValue();
}


std::ostream& operator<<(std::ostream& os, const Boolean& input) {
    os << input.getValue();
    return os;
}