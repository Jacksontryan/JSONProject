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

//Complexity: O(n) where n is the length of the line variable

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

                    //fields value is set to currentString
                    field.setValue(currentString);

                    //add the field to the current object
                    objectStack.peek().addField(field);

                    //clear current string
                    currentString.clear();

                    //reset expecting string
                    expectingKey = true;
                }
            }
            //if the next characters are \"
            else if (c == '\\' && line[i+1] == '\"') {

                //add the character to the current string
                currentString += '\"';

                //increment to the next character
                i++;

            }
            //if current character is not " or \" add character to current string
            else {

                currentString += c;

            }

        }

        //if inNumber
        else if (inNumber) {

            //look at current character
            switch (c) {

                //number cases add number
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

                //decimal point case
                case '.':

                    //check for if the number has a decimal point already
                    try {
                        if (currentNumber.find('.') == string::npos) {
                            currentNumber += '.';
                        }
                        //throw runtime error if 2 decimal points
                        else {
                            throw runtime_error("Number cannot have two decimals");
                        }
                    }catch (runtime_error& e) {
                        //print error message
                        cout << e.what() << endl;
                    }
                    break;
                default:
                    //when you hit a non number character, you are out of the number
                    inNumber = false;

                    //increment back one. I don't know why this is needed, I just know it doesn't work correctly without this line
                    i--;

                    //if the last bracket was an array
                    if (bracketStack.peek() == '[') {

                        //if the current number does not have a decimal point
                        if (currentNumber.find('.') == string::npos) {

                            //convert currentNumber to an int
                            int x = stoi(currentNumber);

                            //add the number to the current list
                            listStack.peek().add(listStack.peek().size(),x);

                        }
                        //if the number has a decimal point
                        else {

                            //convert currentNumber to double
                            double x = stod(currentNumber);

                            //add the number to the current list
                            listStack.peek().add(listStack.peek().size(),x);
                        }
                    }

                    //if top on bracket stack is object
                    else {

                        //create a json object field with the key
                        JSONObjectField field = JSONObjectField(key);

                        //if current number does not have a decimal point
                        if (currentNumber.find('.') == string::npos) {

                            //turn currentNumber to an int
                            int x = stoi(currentNumber);

                            //set the field value to the number
                            field.setValue(x);

                            //add field to the top object
                            objectStack.peek().addField(field);
                        }

                        //if there is a decimal point
                        else {

                            //turn currentNumber to a double
                            double x = stof(currentNumber);

                            //set the field value to the number
                            field.setValue(x);

                            //add the field to the top object
                            objectStack.peek().addField(field);
                        }

                    }

                    //clear current string, number and reset expecting key
                    currentString.clear();
                    currentNumber.clear();
                    expectingKey = true;

            }
        }
        //if not in number or in string
        else {

            //look at current character
            switch (c) {

                //number cases, including decimal point and negative sign
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
                case '-':

                    //add the character to currentNumber
                    currentNumber += c;

                    //inNumber is true
                    inNumber = true;
                    break;

                //if the character is "
                case '\"':

                    //start collecting string characters
                    inString = true;
                    break;

                //true cases. Includes uppercase and lowercase true
                case 't':
                case 'T': {

                    //increment to the end of the word true
                    i+=3;

                    //if the current bracket is an array
                    if (bracketStack.peek() == '[') {

                        //add true to the current list
                        listStack.peek().add(listStack.peek().size(),true);

                    }

                    //if json object bracket
                    else {

                        //create a new field using key
                        JSONObjectField field = JSONObjectField(key);

                        //create a true varable and add it to field
                        bool b = true;
                        field.setValue(b);

                        //add the field to the current object
                        objectStack.peek().addField(field);

                    }

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //if the current character is false
                case 'f':
                case 'F':{

                    //increment to the end of the word false
                    i+=4;

                    //if the top bracket is an array
                    if (bracketStack.peek() == '[') {

                        //add false to the current list
                        listStack.peek().add(listStack.peek().size(),false);
                    }

                    //if top of bracket stack is object bracket
                    else {

                        //create a new object field with key
                        JSONObjectField field = JSONObjectField(key);

                        //create a false variable
                        bool b = false;

                        //set field value to false
                        field.setValue(b);

                        //add field to current object
                        objectStack.peek().addField(field);

                    }

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //null check
                case 'n':
                case 'N':{

                    //increment to end of word
                    i+=3;

                    //if top bracket is array bracket
                    if (bracketStack.peek() == '[') {

                        //add null to current list
                        listStack.peek().add(listStack.peek().size(),NULL);
                    }

                    //if top bracket is object bracket
                    else {

                        //create new json object field with key
                        JSONObjectField field = JSONObjectField(key);

                        //set field value to null
                        field.setValue(NULL);

                        //add field to current object
                        objectStack.peek().addField(field);

                    }

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //if current character is an opened object bracket
                case '{': {

                    //create new json object
                    JSONObject newObject = JSONObject();

                    //if currently in an array
                    if (bracketStack.peek() == '[') {

                        //get the current name off of key stack
                        string name = keyStack.peek();

                        //add the current length of the top list to name
                        name+=std::to_string(listStack.peek().size());

                        //set name of the object to name
                        newObject.setName(name);
                    }

                    //if currently in an object
                    else {

                        //set name of object to key
                        newObject.setName(key);

                    }

                    //push the new object onto the stack
                    objectStack.push(newObject);

                    //push an object bracket to the bracket stack
                    bracketStack.push('{');

                    //push key to the key stack
                    keyStack.push(key);

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //if current character is an opening array bracket
                case '[':

                    //push opening array bracket to bracket stack
                    bracketStack.push('[');

                    //push a new list onto list stack
                    listStack.push(List());

                    //push current key onto key stack
                    keyStack.push(key);

                    //while in an array, you never expect a key
                    expectingKey = false;
                    break;

                //if current character is a closing object bracket
                case '}': {

                    //pop off the top of bracket stack
                    char b = bracketStack.pop();

                    //check to make sure the popped bracket is an opening object bracket
                    try {
                        if (b!='{') {
                            throw runtime_error("Critical Error. Unable To Continue Parsing");
                        }
                    }catch (runtime_error& e) {
                        cout << e.what() << endl;
                    }

                    //take the top of object stack and key stack
                    JSONObject newObject = objectStack.pop();
                    string objectKey = keyStack.pop();

                    //if the current bracket is the last bracket of the bracket stack, return the object. Cannot read in the leftover data after this point
                    if (bracketStack.size() == 0) {
                        return newObject;
                    }

                    //if the top of bracket stack is an array bracket
                    if (bracketStack.peek() == '[') {

                        //add the object to the top of list stack
                        listStack.peek().add(listStack.peek().size(),newObject);
                    }

                    //if top of bracket stack is an object bracket
                    else {

                        //create a new object field
                        JSONObjectField field = JSONObjectField(objectKey);

                        //set the value of the field to the object
                        field.setValue(newObject);

                        //if object stack size is 0, reached an early return. Cannot read in the leftover data
                        if (objectStack.size() == 0) {
                            //cout << "EARLY RETURN: stack empty, objectKey=" << objectKey << endl;
                            return newObject;
                        }

                        //add the field to the top of object stack
                        objectStack.peek().addField(field);
                    }

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //if current character is a closing array bracket
                case ']': {

                    //pop the top of bracket stack and check to make sure that it matches
                    char b = bracketStack.pop();
                    try {
                        if (b!='[') {
                            throw runtime_error("Critical Error. Expected [");
                        }
                    }catch (runtime_error& e) {
                        cout << e.what() << endl;
                    }

                    //get top list of list stack
                    List l = listStack.pop();

                    //get top key of key stack
                    string objectKey = keyStack.pop();

                    //if top of bracket stack is an opening array bracket
                    if (bracketStack.peek() == '[') {

                        //add the current list to the top list of list stack
                        listStack.peek().add(listStack.peek().size(),l);

                    }

                    //if the top of bracket stack is an opening object bracket
                    else {

                        //create new field using object key
                        JSONObjectField field = JSONObjectField(objectKey);

                        //set field value to l
                        field.setValue(l);

                        //add field to top object of object stack
                        objectStack.peek().addField(field);

                    }

                    //reset expecting key
                    expectingKey = true;
                    break;
                }

                //if character is :
                case ':':

                    try {
                        if (!expectingKey) {
                            throw runtime_error("Caught expected key");
                        }
                    }catch (runtime_error& e) {
                        cout << e.what() << endl;
                    }

                    //not expecting key
                    expectingKey = false;

                    //clear current string
                    currentString.clear();
                    break;

                //skip commas
                case ',':
                    break;

                //default should never trigger. Triggering probably means a syntax error
                default:
                    cout << "Default Triggered. Should not have happened" << endl;
            }
        }

        //move onto next character
        i++;
    }

    //return top object. should be last object
    return objectStack.pop();
}

ArrayList<JSONObject> vectorRoot(string line){
    return ArrayList<JSONObject>();
}

int main() {
    string line = readFile("/home/jacksonryan/CLionProjects/JSONProject/cmake-build-debug/Resources/data/a.json");
    JSONObject object = objectRoot(line);

    cout << object.getName() << " " << object.getFields().size() << endl;
    for (int i = 0; i < object.getFields().size(); i++) {
        cout << object.getFields().get(i).getName() << endl;
    }

    return 0;
}