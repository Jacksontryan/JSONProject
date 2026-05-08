cmake_minimum_required(VERSION 4.0)
project(JSONProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(JSONProject
        DataStructures/ArrayList.h
        DataStructures/Collection.h
        DataStructures/List.h
        DataStructures/Node.h
        DataStructures/Stack.cpp
        JSON/JSONFileReader.cpp
        JSON/JSONObject.cpp
        JSON/JSONObjectField.cpp)
