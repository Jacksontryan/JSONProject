//
// Created by jacks on 4/21/2026.
//
#include <fstream>
#include <iostream>

#include "JSONObjectField.h"
#include "JSONObject.h"
#include <vector>

#include "../DataStructures/Stack.h"
#include "../DataStructures/Stack.cpp"

#include "../DataStructures/ArrayList.h"
#include "../DataStructures/List.h"

using namespace std;

string readFile(string filename) {
    ifstream file(filename);
    string line;
    vector<JSONObject> objects;
    char c;
    int balance = 0;
    bool inString = false;
    while (file.get(c)) {
        if (inString) {
            if (c == '\\') {
                file.get(c);
                if (c != 'r' && c != 'n' && c != 't' && c != 'f') {
                    line.push_back('\\');
                    line.push_back(c);
                }
            }else if (c == '\"') {
                line.push_back('\"');
                inString = false;
            }else {
                line.push_back(c);
            }
        }else {
            if (c == '\"') {
                line.push_back('\"');
                inString = true;
            }else if (c == '{' || c == '[') {
                balance++;
                line.push_back(c);
            }else if (c == '}' || c == ']') {
                line.push_back(c);
                balance--;
            }else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                line.push_back(c);
            }
        }
    }

    try {
        if (line.empty()) {
            throw runtime_error("Empty JSON file!");
        }if (balance != 0) {
            throw runtime_error("Brackets do not match up");
        }
    }catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    return line;
}

JSONObject objectRoot(string line) {

    //cout << line << endl;

    int depth = 1;
    int i = 1;

    char c;

    JSONObject root = JSONObject("Root");
    JSONObject current;

    bool inString = false;
    bool inNumber = false;
    bool expectingKey = true;

    string currentString;
    string key;
    string currentNumber;

    Stack<char> bracketStack;
    bracketStack.push('{');

    Stack<JSONObject> stack;
    stack.push(root);

    Stack<string> keyStack;
    keyStack.push("root");

    Stack<List> listStack;
    List currentList;

    while (i < line.length()) {

        c = line[i];

        //cout << "i=" << i << " c=" << c << endl;

        if (inString) {

            if (c == '\"') {

                inString = false;

                if (expectingKey) {

                    key = currentString;
                    currentString.clear();
                    expectingKey = false;

                }else if (bracketStack.peek() == '[') {
                    listStack.peek().add(listStack.peek().size(),currentString);
                    currentString.clear();
                    expectingKey = true;
                }else {
                    JSONObjectField field = JSONObjectField(key);
                    field.setValue(currentString);
                    //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                    stack.peek().addField(field);
                    //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                    currentString.clear();
                    expectingKey = true;
                }
            }else if (c == '\\' && line[i+1] == '\"') {

                currentString += '\"';
                i++;

            }else {

                currentString += c;

            }
        }else if (inNumber) {
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    currentNumber += c;
                    break;
                case '.':
                    try {
                        if (currentNumber.find('.') == string::npos) {
                            currentNumber += '.';
                        }else {
                            throw runtime_error("Number cannot have two decimals");
                        }
                    }catch (runtime_error& e) {
                        cout << e.what() << endl;
                    }
                    break;
                default:
                    inNumber = false;
                    i--;
                    if (bracketStack.peek() == '[') {
                        if (currentNumber.find('.') == string::npos) {
                            int x = stoi(currentNumber);
                            listStack.peek().add(listStack.peek().size(),x);
                        }else {
                            double x = stod(currentNumber);
                            listStack.peek().add(listStack.peek().size(),x);
                        }
                    }else {
                        JSONObjectField field = JSONObjectField(key);
                        if (currentNumber.find('.') == string::npos) {
                            int x = stoi(currentNumber);
                            field.setValue(x);
                            //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                            stack.peek().addField(field);
                            //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                        }else {
                            double x = stof(currentNumber);
                            field.setValue(x);
                            //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                            stack.peek().addField(field);
                            //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                        }
                    }
                    currentString.clear();
                    currentNumber.clear();
                    expectingKey = true;
            }
        }else {
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '.':
                    currentNumber += c;
                    inNumber = true;
                    break;
                case '\"':
                    inString = true;
                    break;
                case 't':
                case 'T': {
                    i+=3;
                    if (bracketStack.peek() == '[') {
                        listStack.peek().add(listStack.peek().size(),true);
                    }else {
                        JSONObjectField field = JSONObjectField(key);
                        bool b = true;
                        field.setValue(b);
                        //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                        stack.peek().addField(field);
                        //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                    }
                    expectingKey = true;
                    break;
                }
                case 'f':
                case 'F':{
                    i+=4;
                    //cout << "false detected at i=" << i << " next chars: "
         //<< line[i+1] << line[i+2] << line[i+3] << line[i+4] << endl;
                    if (bracketStack.peek() == '[') {
                        listStack.peek().add(listStack.peek().size(),false);
                    }else {
                        JSONObjectField field = JSONObjectField(key);
                        bool b = false;
                        field.setValue(b);
                        //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                        stack.peek().addField(field);
                        //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                    }
                    expectingKey = true;
                    break;
                }
                case 'n':
                case 'N':{
                    i+=3;
                    if (bracketStack.peek() == '[') {
                        listStack.peek().add(listStack.peek().size(),NULL);
                    }else {
                        JSONObjectField field = JSONObjectField(key);
                        field.setValue(NULL);
                        stack.peek().addField(field);
                    }
                    expectingKey = true;
                    break;
                }
                case '{': {
                    JSONObject newObject = JSONObject();
                    stack.push(newObject);
                    bracketStack.push('{');
                    keyStack.push(key);
                    expectingKey = true;
                    break;
                }
                case '[':
                    bracketStack.push('[');
                    listStack.push(List());
                    keyStack.push(key);
                    expectingKey = false;
                    break;
                case '}': {
                    char b = bracketStack.pop();
                    try {
                        if (b!='{') {
                            throw runtime_error("Critical Error. Unable To Continue Parsing");
                        }
                    }catch (runtime_error& e) {
                        cout << e.what() << endl;
                    }
                    JSONObject newObject = stack.pop();
                    string objectKey = keyStack.pop();

                    //cout << "} hit: objectKey=" << objectKey
         //<< " bracketStack.size()=" << bracketStack.size()
         //<< " stack.size()=" << stack.size() << endl;

                    if (bracketStack.size() == 0) {
                        //cout << "Early return triggered at i=" << i << " objectKey=" << objectKey << endl;
                        return newObject;
                    }
                    if (bracketStack.peek() == '[') {
                        listStack.peek().add(listStack.peek().size(),newObject);
                    }else {
                        JSONObjectField field = JSONObjectField(objectKey);
                        field.setValue(newObject);
                        if (stack.size() == 0) {
                            //cout << "EARLY RETURN: stack empty, objectKey=" << objectKey << endl;
                            return newObject;
                        }
                        //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                        stack.peek().addField(field);
                        //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                    }
                    expectingKey = true;
                    break;
                }
                case ']': {
                    bracketStack.pop();
                    List l = listStack.pop();
                    string objectKey = keyStack.pop();
                    if (bracketStack.peek() == '[') {
                        listStack.peek().add(listStack.peek().size(),l);
                    }else {
                        JSONObjectField field = JSONObjectField(objectKey);
                        field.setValue(l);
                        //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                        stack.peek().addField(field);
                        //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                    }
                    expectingKey = true;
                    break;
                }case ':':
                    expectingKey = false;
                    currentString.clear();
                    break;
                case ',':
                    break;
                default:
                    cout << "Default Triggered. Should not have happened" << endl;
            }
        }
        i++;
    }
    return stack.pop();
}

vector<JSONObject> vectorRoot(string line){
    return vector<JSONObject>();
}

vector<JSONObject> instantiateFile(const string& filename) {

    string line = readFile(filename);

    //at this point, line is a single string without any whitespaces(' ', '\n', '\t', '\r')

    vector<JSONObject> objects;

    try {
        if (line[0] == '{') {
            JSONObject root = objectRoot(line);
            objects.push_back(root);
            return objects;
        }else if (line[0] == '[') {
            objects = vectorRoot(line);
        }else {
            string error = "JSON file must start with [ or { not ";
            error += line[0];
            throw runtime_error(error);
        }
    }catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    return objects;

}


int main() {
    string line = readFile("C:/Users/jacks/CLionProjects/videoGame/cmake-build-debug/Resources/sample.json");
    JSONObject object = objectRoot(line);
    cout << object.getName() << " " << object.getFields().size() << endl;
    for (int i = 0; i < object.getFields().size(); i++) {
        cout << object.getFields().get(i).getName() << endl;
    }
    JSONObject inner = object.getFields().get(5).getValue<JSONObject>();
    cout << inner.getName() << " " << inner.getFields().size() << endl;
    for (int i = 0; i < inner.getFields().size(); i++) {
        cout << inner.getFields().get(i).getName() << endl;
    }
    return 0;
}