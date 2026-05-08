//
// Created by jacks on 4/20/2026.
//

#include "JSONObject.h"

JSONObject::JSONObject() {
    this->name = "";
    this->parent = nullptr;
}

JSONObject::JSONObject(string name) {
    this->name = name;
    this->parent = nullptr;
}

JSONObject::JSONObject(const JSONObject &other) {
    this->name = other.name;
    this->parent = other.parent;
    this->fields = other.fields;
}

JSONObject &JSONObject::operator=(const JSONObject &other) {
    this->name = other.name;
    this->parent = other.parent;
    this->fields = other.fields;
    return *this;
}


bool JSONObject::addField(JSONObjectField field) {
    for (int i = 0; i < fields.size(); i++) {
        if (fields.get(i).equals(&field)) {
            return false;
        }
    }
    fields.addLast(field);
    return true;
}

ArrayList<JSONObjectField>& JSONObject::getFields() {
    return fields;
}

JSONObjectField JSONObject::getField(string name) {
    for (int i = 0; i < fields.size(); i++) {
        if (fields.get(i).getName() == name) {
            return fields.get(i);
        }
    }
    return JSONObjectField("Null Field");
}

string JSONObject::getName() {
    return name;
}

void JSONObject::setName(string name) {
    this->name = name;
}

void JSONObject::setParent(JSONObject *parent) {
    this->parent = parent;
}

JSONObject* JSONObject::getParent() const {
    return parent;
}
