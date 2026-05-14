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

//requires: string name of file path and name of a correctly formatted .json file

//returns: a string value that represents the entire .json file in 1 line with no whitespace or new line characters

//CHECKS:
//1.if the number of opening brackets matches the number of closing brackets
//2. if the file is empty
//3. if the file is not there

//DOES NOT CHECK:
//1. if the number of { brackets equals the number of } brackets or if the number of [ brackets equals the number of ] brackets
//2. if there is a syntax error
//3. if the .json file starts with an opening bracket
//4. if closing brackets come before opening brackets

//complexity: 0(n) where n is the length of the json file

string readFile(string filename) {

    //file reader
    ifstream file(filename);

    //string line of the deflated .json file
    string line;

    //current character
    char c;

    //balance variable. makes sure the number of opening brackets {, [ equals the number of closing brackets }, ]
    int balance = 0;

    //bool value that checks if we are in a string. while in string, special checks are necessary for special characters
    bool inString = false;

    //for each character in the file
    while (file.get(c)) {

        //if currently in a string
        if (inString) {

            //special check \ character
            if (c == '\\') {

                //get the next character
                file.get(c);

                //if character is not a whitespace, add it to the line
                if (c != 'r' && c != 'n' && c != 't' && c != 'f') {
                    line.push_back('\\');
                    line.push_back(c);
                }

            }

            //special check "
            else if (c == '\"') {

                //add quotation mark to line
                line.push_back('\"');

                //because you exited quotation mark, inString is set to false
                inString = false;

            }else {//if not a special character, add it to the end of line
                line.push_back(c);
            }

        }else {//if not in string

            //special check "
            if (c == '\"') {

                //add quotation mark to line
                line.push_back('\"');

                //because you have entered quotation marks, set inString to true
                inString = true;

            }

            //if entering a bracket, add 1 to balance and add the character to line
            else if (c == '{' || c == '[') {
                balance++;
                line.push_back(c);
            }

            //if exiting bracket, subtract 1 from balance and add the character to line
            else if (c == '}' || c == ']') {
                line.push_back(c);
                balance--;
            }

            //if character is not a whitespace or new line character, add it to the back
            else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                line.push_back(c);
            }

        }

    }

    //if the line is empty, or balance is off throw error
    try {
        if (line.empty()) {
            throw runtime_error("Empty JSON file!");
        }if (balance != 0) {
            throw runtime_error("Brackets do not match up");
        }
    }catch (runtime_error& e) {
        cout << e.what() << endl;
    }

    //return the line
    return line;
}

//requires: non-empty, balanced deflated json string

//returns: Root JSON Object with all fields complete
JSONObject objectRoot(string line) {

    //the first character is guaranteed to be {
    //as such, the depth is guaranteed to start at 1 and start indexing at the second character
    int depth = 1;
    int i = 1;

    //current character
    char c;

    //Root object, or the object to be returned
    JSONObject root = JSONObject("Root");

    //in string check
    bool inString = false;

    //in number check
    bool inNumber = false;

    //Check for expecting key. A key is expected before the : character
    bool expectingKey = true;

    //current string you are dealing with
    string currentString;

    //current key you are dealing with. Key is the name of the object
    string key;

    //current number string to be parsed later
    string currentNumber;

    //stack of opening brackets. When poping brackets, if the popped bracket does not correspond to the current closing bracket, throws an error
    Stack<char> bracketStack;

    //push { onto bracketStack to represent the root
    bracketStack.push('{');

    //an object stack. Top object is the parent of whatever you are currently dealing with
    Stack<JSONObject> objectStack;

    //push root object to the object stack
    objectStack.push(root);

    //stack of keys, or names of the objects and fields
    Stack<string> keyStack;

    //push root onto the keyStack
    keyStack.push("Root");

    //stack of lists. I had to create a generic list class that is a total mess if you try to add multiple types of objects, but the functionality is there
    Stack<List> listStack;

    //current list you are dealing with
    List currentList;


    //loop through the deflated .json file, starting with the character after the {
    while (i < line.length()) {

        //get the current character
        c = line[i];

        //if currently in a string
        if (inString) {

            //if current character is a closing parentheses
            if (c == '\"') {

                //set inString to false
                inString = false;

                //if currently expecting a key
                if (expectingKey) {

                    //switch currentString to key
                    key = currentString;

                    //clear current string for future use
                    currentString.clear();

                    //set expectingKey to false
                    expectingKey = false;

                }

                //if not expecting key, but the top of bracketStack is a list
                else if (bracketStack.peek() == '[') {

                    //add the current string to the top list of listStack
                    listStack.peek().add(listStack.peek().size(),currentString);

                    //clear currentString
                    currentString.clear();

                    //set expecting key to false
                    expectingKey = false;

                }else {//if not expecting key and not in a list

                    //create new JSONObjectField
                    JSONObjectField field = JSONObjectField(key);
                    field.setValue(currentString);
                    objectStack.peek().addField(field);
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
                            objectStack.peek().addField(field);
                            //cout << "stack.peek() now has " << stack.peek().getFields().size() << " fields" << endl;
                        }else {
                            double x = stof(currentNumber);
                            field.setValue(x);
                            //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                            objectStack.peek().addField(field);
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
                        objectStack.peek().addField(field);
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
                        objectStack.peek().addField(field);
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
                        objectStack.peek().addField(field);
                    }
                    expectingKey = true;
                    break;
                }
                case '{': {
                    JSONObject newObject = JSONObject();
                    if (bracketStack.peek() == '[') {
                        string name = keyStack.peek();
                        name+=std::to_string(listStack.peek().size());
                        newObject.setName(name);
                    }else {
                        newObject.setName(key);
                    }
                    objectStack.push(newObject);
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
                    JSONObject newObject = objectStack.pop();
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
                        if (objectStack.size() == 0) {
                            //cout << "EARLY RETURN: stack empty, objectKey=" << objectKey << endl;
                            return newObject;
                        }
                        //cout << "Adding field '" << key << "' to stack.peek() at address " << &stack.peek() << endl;
                        objectStack.peek().addField(field);
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
                        objectStack.peek().addField(field);
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
    return objectStack.pop();
}

ArrayList<JSONObject> vectorRoot(string line){
    return ArrayList<JSONObject>();
}

int main() {
    string line = readFile("C:/Users/jacks/CLionProjects/videoGame/cmake-build-debug/Resources/sample.json");
    JSONObject object = objectRoot(line);

    cout << object.getName() << " " << object.getFields().size() << endl;
    for (int i = 0; i < object.getFields().size(); i++) {
        cout << object.getFields().get(i).getName() << endl;
    }

    cout << endl;

    JSONObject inner = object.getFields().get(5).getValue<JSONObject>();
    cout << inner.getName() << " " << inner.getFields().size() << endl;
    for (int i = 0; i < inner.getFields().size(); i++) {
        cout << inner.getFields().get(i).getName() << endl;
    }

    cout << endl;

    List departments = object.getFields().get(6).getValue<List>();
    cout << object.getFields().get(6).getName() << " " << departments.size() << endl;
    for (int i = 0; i < departments.size(); i++) {
        cout << departments.get<string>(i) << endl;
    }

    cout << endl;

    List employees = object.getFields().get(7).getValue<List>();
    cout << employees.size() << endl;

    for (int i = 0; i < employees.size(); i++) {

        JSONObject o = employees.get<JSONObject>(i);
        cout << o.getName() <<  endl;

        int id = o.getField("id").getValue<int>();
        cout << "id: " << id << endl;

        string name = o.getField("name").getValue<string>();
        cout << "name: " << name << endl;

        string role = o.getField("role").getValue<string>();
        cout << "role: " << role << endl;

        int salary = o.getField("salary").getValue<int>();
        cout << "salary: " << salary << endl;

        bool remote = o.getField("remote").getValue<bool>();
        cout << "remote: " << remote << endl;

        cout << endl;
    }

    JSONObject benifits = object.getField("benefits").getValue<JSONObject>();
    cout << benifits.getName() << endl;

    bool health_insurance = benifits.getField("health_insurance").getValue<bool>();
    cout << "health_insurance: " << health_insurance << endl;
    bool dental = benifits.getField("dental").getValue<bool>();
    cout << "dental: " << dental << endl;
    string retirement_plan = benifits.getField("retirement_plan").getValue<string>();
    cout << "retirement_plan: " << retirement_plan << endl;
    int pto_days = benifits.getField("pto_days").getValue<int>();
    cout << "pto_days: " << pto_days << endl;
    int parental_leave_weeks = benifits.getField("parental_leave_weeks").getValue<int>();
    cout << "parental_leave_weeks: " << parental_leave_weeks << endl;

    return 0;
}