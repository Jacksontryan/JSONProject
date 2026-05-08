//
// Created by jacks on 4/27/2026.
//

#ifndef VIDEOGAME_STACK_H
#define VIDEOGAME_STACK_H
#include "ArrayList.h"

template<class T>
class Stack : public Collection<T>{
    ArrayList<T> *array;
    public:
    Stack();
    ~Stack();
    T pop();
    void push(T element);
    T& peek();
    void clear();
    ArrayList<T>* toArray();
    int size();
};

//#include "Stack.cpp"

#endif //VIDEOGAME_STACK_H