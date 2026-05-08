//
// Created by jacks on 4/20/2026.
//

#include "JSONObjectField.h"
#include <string>

JSONObjectField::JSONObjectField() {
    this->name="Untitled Field";
    this->type=nullptr;
}

JSONObjectField::JSONObjectField(string name) {
    this->name = name;
    this->type = nullptr;
}

const type_info *JSONObjectField::getClass() const {
    return this->type;
}

string JSONObjectField::getName() const {
    return this->name;
}

void JSONObjectField::setName(string name) {
    this->name = name;
}


bool JSONObjectField::equals(JSONObjectField *field) {
    return field->getName() == this->name && field->getClass() == this->type;
}

bool JSONObjectField::operator==(JSONObjectField *field) {
    return this->equals(field);
}