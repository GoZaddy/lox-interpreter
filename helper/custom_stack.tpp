template <typename T>
void Stack<T>::push(const T& value) {
    elements.push_back(value);
}

template <typename T>
T Stack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Stack<T> is empty");
    }

    T topElement = elements.back();
    elements.pop_back();
    return topElement;
}

template <typename T>
bool Stack<T>::empty() const {
    return elements.empty();
}

template <typename T>
T Stack<T>::at(size_t index) const {
    if (index >= size()) {
        throw std::out_of_range("Index out of bounds");
    }

    return elements[index];
}

template <typename T>
T Stack<T>::peek() const {
    return at(size()-1);
}

template <typename T>
size_t Stack<T>::size() const {
    return elements.size();
}
