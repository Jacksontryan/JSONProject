//
// Created by jacks on 4/20/2026.
//

#ifndef VIDEOGAME_JSONOBJECT_H
#define VIDEOGAME_JSONOBJECT_H
#include <any>
#include <vector>

#include "JSONObjectField.h"
#include "../DataStructures/ArrayList.h"

using namespace std;

class JSONObject {
    string name;
    ArrayList<JSONObjectField> fields;
    JSONObject* parent;

    public:
    JSONObject();
    explicit JSONObject(string name);
    JSONObject(const JSONObject& other);
    JSONObject& operator=(const JSONObject& other);
    bool addField(JSONObjectField field);
    ArrayList<JSONObjectField>& getFields();
    JSONObjectField getField(string name);
    string getName();
    void setName(string name);
    void setParent(JSONObject* parent);
    JSONObject* getParent() const;
};

//#include "JSONObject.cpp"
#endif //VIDEOGAME_JSONOBJECT_H