//
// Created by jacks on 4/20/2026.
//

#ifndef VIDEOGAME_JSONOBJECTFIELD_H
#define VIDEOGAME_JSONOBJECTFIELD_H

#include <any>
#include <string>
#include <typeinfo>

using namespace std;


class JSONObjectField {

    string name;
    any value;
    const type_info* type;

    public:
    JSONObjectField();
    explicit JSONObjectField(string name);
    [[nodiscard]] string getName() const;
    void setName(string name);
    [[nodiscard]] const type_info* getClass() const;
    template<class T>
    void setValue(T value){
        this->type = &typeid(T);
        this->value = value;
    }
    template<class T>
    T getValue() {
        return any_cast<T>(value);
    }

    bool equals(JSONObjectField* field);
    bool operator==(JSONObjectField* field);

};

#include "JSONObjectField.h"

#endif //VIDEOGAME_JSONOBJECTFIELD_H