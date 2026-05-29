//
// Created by jacks on 5/3/2026.
//

#ifndef VIDEOGAME_LIST_H
#define VIDEOGAME_LIST_H
#include "Collection.h"
#include <any>
#include <stdexcept>
using namespace std;
class List {
    any*array;
    int capacity;
    int length;
public:

    List() {
        array = new any[10];
        capacity = 10;
        length = 0;
    }

    List(const List& other) {
        capacity = other.capacity;
        length = other.length;
        array = new any[capacity];
        for (int i = 0; i < length; i++) {
            array[i] = other.array[i];
        }
    }

    List& operator=(const List& other) {
        if (this == &other) return *this;
        delete[] array;
        capacity = other.capacity;
        length = other.length;
        array = new any[capacity];
        for (int i = 0; i < length; i++) {
            array[i] = other.array[i];
        }
        return *this;
    }

    ~List() {
        delete [] array;
    }

    template <class T>
    void add(int pos, T value) {
        if (length >= capacity) {
            capacity = length + 10;
            any*temp = new any[capacity];
            for (int i = 0; i < length; i++) {
                temp[i] = array[i];
            }
            delete [] array;
            array = temp;
        }
        if (pos == length) {
            length++;
            array[pos] = value;
        }else {
            for (int i = length - 1; i >= pos; i--) {
                array[i + 1] = array[i];
            }
            array[pos] = value;
            length++;
        }
    }

    template <class T>
    T& remove(int pos) {
        if (pos < 0 || pos >= length) {
            throw runtime_error("index out of range");
        }
        if (pos == length - 1) {
            length--;
            return std::any_cast<T&>(array[pos]);
        }else {
            T value = std::any_cast<T&>(array[pos]);
            for (int i = pos; i < length - 1; i++) {
                array[i] = array[i + 1];
            }
            length--;
            return value;
        }
    }

    template <class T>
    T& get(int pos) {
        if (pos < 0 || pos >= length) {
            throw runtime_error("index out of range");
        }
        return std::any_cast<T&>(array[pos]);
    }

    int size() const {
        return length;
    }

    void clear() {
        length = 0;
    }

    bool checkSame() const{

        if (this->length <= 1) return true;
        const type_info& info = array[0].type();
        for (int i = 0; i < length; i++) {
            if (array[i].type() != info) {
                return false;
            }
        }
        return true;
    }

    template <class T>
    bool allSameType() const {
        if (this->length <= 1) return true;
        for (int i = 0; i < length; i++) {
            if (array[i].type() != typeid(T)) {
                return false;
            }
        }
        return true;
    }

    const type_info& getCommonType() const {
        if (this->length <= 1) return array[0].type();
        return typeid(bool);
    }

    template <class T>
    ArrayList<T> toArrayList() const {
        ArrayList<T> list;
        if (!allSameType<T>()) {
            return list;
        }
        for (int i = 0; i < length; i++) {
            list.add(i, std::any_cast<T>(array[i]));
        }
        return list;
    }


};


#endif //VIDEOGAME_LIST_H