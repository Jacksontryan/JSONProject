//
// Created by jacks on 4/27/2026.
//

#include "Stack.h"
#include<vector>

using namespace std;

template<class T>
Stack<T>::Stack() {
    this->length = 0;
    this->array = new ArrayList<T>();
}

template<class T>
Stack<T>::~Stack() {

}

template<class T>
void Stack<T>::push(T element) {
    this->array->addLast(element);
    ++this->length;
}

template<class T>
T Stack<T>::pop() {
    T value = array->removeLast();
    --this->length;
    return value;
}

template<class T>
T& Stack<T>::peek() {
    return array->get(this->size() - 1);
}

template<class T>
void Stack<T>::clear() {
    this->array->clear();
}

template<class T>
ArrayList<T>* Stack<T>::toArray() {
    return this->array;
}

template<class T>
int Stack<T>::size() {
    return this->array->size();
}

