//
// Created by jacks on 4/26/2026.
//

#ifndef VIDEOGAME_ARRAYLIST_H
#define VIDEOGAME_ARRAYLIST_H
#include <string>

#include "Collection.h"

using namespace std;

template <class T>
class ArrayList : public Collection<T> {
    int capacity = 0;
    int capacityIncrement;
    T* array;
    public:
    ArrayList(){
        this->length = 0;
        this->array = new T[10]();
        this->capacityIncrement = 10;
        this->capacity+=capacityIncrement;
    };
    ArrayList(T* array, int length){
        this->array = array;
        this->length = length;
        this->capacityIncrement = 10;
        this->capacity += capacityIncrement;
    };
    ArrayList(const ArrayList& other) {
        this->length = other.length;
        this->capacity = other.capacity;
        this->capacityIncrement = other.capacityIncrement;
        this->array = new T[this->capacity]();
        for (int i = 0; i < this->length; i++) {
            this->array[i] = other.array[i];
        }
    }

    ArrayList& operator=(const ArrayList& other) {
        if (this == &other) return *this;
        delete[] this->array;
        this->length = other.length;
        this->capacity = other.capacity;
        this->capacityIncrement = other.capacityIncrement;
        this->array = new T[this->capacity]();
        for (int i = 0; i < this->length; i++) {
            this->array[i] = other.array[i];
        }
        return *this;
    }

    ~ArrayList() { delete[] this->array; }

    bool setCapacityIncrement(int increment){
        this->capacityIncrement = increment;
        return true;
    };

    void add(int pos, T element){
        this->length++;
        if (this->length >= this->capacity) {
            while (this->length >= this->capacity) {
                this->capacity += this->capacityIncrement;
            }
            T* temp = new T[this->capacity];
            for (int i = 0; i < this->length; i++) {
                temp[i] = this->array[i];
            }
            delete [] this->array;
            this->array = temp;
        }
        if (pos == this->length - 1) {
            this->array[pos] = element;
        }else if (pos > this->length - 1) {
            for (int i = this->length - 1; i < pos; i++) {
                this->array[i] = T();
            }
            this->array[pos] = element;
            this->length = pos + 1;
        }else if (pos == 0) {
            for (int i = this->length; i > pos; i--) {
                this->array[i] = this->array[i - 1];
            }
            this->array[0] = element;
        }else if (pos > 0 && pos < this->length) {
            for (int i = this->length; i > pos; i--) {
                this->array[i] = this->array[i - 1];
            }
            this->array[pos] = element;
        }
    };
    void addFirst(T element){
        this->add(0, element);
    }
    void addLast(T element){
        this->add(this->length, element);
    }

    T remove(int pos){
        T value;
        if (pos == 0) {
            value = this->array[0];
            for (int i = 0; i < this->length - 1; i++) {
                this->array[i] = this->array[i + 1];
            }
            --this->length;
            return value;
        }else if (pos == this->length - 1) {
            value = this->array[this->length - 1];
            --this->length;
            return value;
        }else if (pos > 0 && pos < this->length) {
            value = this->array[pos];
            for (int i = pos; i < this->length - 1; i++) {
                this->array[i] = this->array[i + 1];
            }
            --this->length;
            return value;
        }
        return value;
    }
    bool removeValue(T value){
        for (int i = 0; i < this->length; i++) {
            if (this->array[i] == value) {
                this->remove(i);
                return true;
            }
        }
        return false;
    }
    T removeLast(){
        return this->remove(this->length - 1);
    }
    T removeFirst(){
        return this->remove(0);
    }

    T& get(int pos){
        return this->array[pos];
    }
    T& getFirst(){
        return this->array[0];
    }
    T& getLast(){
        return this->array[this->length - 1];
    }

    T* find(T element){
        for (int i = 0; i < this->length; i++) {
            if (this->array[i] == element) {
                return &this->array[i];
            }
        }
        return nullptr;
    }
    bool contains(T element){
        for (int i = 0; i < this->length; i++) {
            if (this->array[i] == element) {
                return true;
            }
        }
        return false;
    }
    int indexOf(T element){
        for (int i = 0; i < this->length; i++) {
            if (this->array[i] == element) {
                return i;
            }
        }
        return -1;
    }

    void clear(){
        for (int i = 0; i < this->length; i++) {
            this->array[i] = NULL;
        }
        this->length = 0;
        this->capacity = 10;
    }

    string toString(){
        string s;
        for (int i = 0; i < this->length; i++) {
            s += this->array[i];
        }
        return s;
    }

};

#endif //VIDEOGAME_ARRAYLIST_H