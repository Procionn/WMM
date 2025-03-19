#include "wmml.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

wmml::wmml (std::filesystem::path file) : targetFile(file, std::ios::binary | std::ios::in | std::ios::out) {
    if (!targetFile.is_open()) {
        targetFile.open(file, std::ios::binary | std::ios::in | std::ios::out);
        if (!targetFile.is_open())
            abcerr("In constructor exists file: " + file.generic_string() + " is not found");
    }
    else {
        targetFile.seekp(0);
        targetFile.read(reinterpret_cast<char*>(&ObjectsInThread), sizeof(unsigned short int));
    }
    filename = file.generic_string();
}
wmml::wmml (std::filesystem::path file, int OIT) : targetFile(file, std::ios::binary | std::ios::in | std::ios::out) {
    if (!targetFile.is_open()) {
        ObjectsInThread = OIT;
        std::ofstream File(file, std::ios::binary);
        File.close();
        targetFile.open(file, std::ios::binary | std::ios::in | std::ios::out);
        targetFile.seekp(0);
        targetFile.write(reinterpret_cast<const char*>(&ObjectsInThread), sizeof(ObjectsInThread));
    }
    else {
        targetFile.seekp(0);
        targetFile.read(reinterpret_cast<char*>(&ObjectsInThread), sizeof(unsigned short int));
        abcerr("In constructor not exists file: " + file.generic_string() + " is exists");
    }
    filename = file.generic_string();
}

wmml::~wmml() {
    targetFile.close();
}

unsigned short int wmml::size() {return ObjectsInThread;}


bool wmml::read (std::vector<std::string>& out) {
    if (!targetFile.is_open()) std::cerr << "Couldn't open the file for reading!" << std::endl;
    unsigned short int stringSize;
    if (targetFile.eof()) return false;
    else {
        for (int num = 0; num != ObjectsInThread; ++num ) {
            targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
            if (targetFile.eof()) return false;
            std::string str(stringSize, '\0');
            targetFile.read(&str[0], stringSize);
            out[num] = str;
        }
    }
    return true;
}
bool wmml::read () {
    if (!targetFile.is_open()) std::cerr << "Couldn't open the file for reading!" << std::endl;
    unsigned short int stringSize;
    if (targetFile.eof()) return false;
    else {
        for (int num = 0; num != ObjectsInThread; ++num ) {
            targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
            if (targetFile.eof()) return false;
            std::string str(stringSize, '\0');
            targetFile.read(&str[0], stringSize);
        }
    }
    return true;
}



void wmml::add (std::vector<std::string>& in) {
    if (targetFile.eof()) targetFile.clear();
    targetFile.seekp(0, std::ios::end);
    if (!targetFile.is_open()) std::cerr << "Couldn't open the file for additional recording!" << std::endl;
    for (std::string inputString : in) {
        unsigned short int stringSize = inputString.size();
        targetFile.write(reinterpret_cast<const char*>(&stringSize), sizeof(stringSize));
        targetFile.write(inputString.data(), stringSize);
    }
}


void wmml::reset() {
    targetFile.clear();
    targetFile.seekp(sizeof(unsigned short int));
}

int wmml::sizeRequest() {
    int counter = 0;
    while (read())
        ++counter;
    return counter;
}

void wmml::overwriting (int tag, int field, std::string newStr) {
    if (field > ObjectsInThread) 
        abcerr("this fields does not exist in object!");
    for (;tag != 0; --tag) {
        if (!read()) 
            abcerr("this object does not exist in file!");
    }
    unsigned short int stringSize;
    for (; field != 0; --field) {
        targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
        if (targetFile.eof()) 
            abcerr("the end of the file is reached before the end of the cycle!");
        std::string str(stringSize, '\0');
        targetFile.read(&str[0], stringSize);
    }
    if (targetFile.eof()) 
        abcerr("the end of the file is reached before the end of the cycle!");
    targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
    if (stringSize == newStr.size()) {
        targetFile.write(newStr.data(), stringSize);
    }
    else {
        abcerr("the size of the new value does not match the size of the field being overwritten!");
    }
}

bool wmml::skip (int counter) {
    if (!targetFile.is_open()) abcerr("file is not opened");
    unsigned short int stringSize = 0;
    if (targetFile.eof()) return false;
    else {
        while (counter != 0) {
            targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
            targetFile.seekp(static_cast<int>(targetFile.tellg()) + stringSize);
            if (targetFile.eof()) return false;
            --counter;
        }
        return true;
    }
}
std::string wmml::read_field () {
    if (!targetFile.is_open()) abcerr("file is not opened");
    unsigned short int stringSize;
    if (targetFile.eof()) return NULL;
    else {
        targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
        std::string out(stringSize, '\0');
        targetFile.read(&out[0], stringSize);
        return out;
    }
    return NULL;
}

bool wmml::skip_sector (int counter) {
    int i = ObjectsInThread * counter;
    if (skip(i)) return true;
    return false;
}

void wmml::replace (int& tag, std::vector<std::string>& in) {
    reset();
    int fn = sizeRequest();
    std::vector<std::vector<std::string>> gv(fn);
    reset();
    for (int i = 0; i != fn; ++i) {
        std::vector<std::string> v(ObjectsInThread);
        read(v);
        gv[i] = v;
    }
    gv[tag] = in;
    targetFile.close();
    std::ofstream rmfile(filename.c_str(), std::ios::binary | std::ios::trunc);
    rmfile.close();
    targetFile.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    targetFile.seekp(0);
    targetFile.write(reinterpret_cast<const char*>(&ObjectsInThread), sizeof(ObjectsInThread));
    for (std::vector<std::string> v : gv) {
        add(v);
    }
}

void wmml::remove (int& tag) {
    reset();
    int fn = sizeRequest();
    std::vector<std::vector<std::string>> gv(fn);
    reset();
    for (int i = 0; i != fn; ++i) {
        if (i == tag) continue;
        std::vector<std::string> v(ObjectsInThread);
        read(v);
        gv[i] = v;
    }
    targetFile.close();
    std::ofstream rmfile(filename.c_str(), std::ios::binary | std::ios::trunc);
    rmfile.close();
    targetFile.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    targetFile.seekp(0);
    targetFile.write(reinterpret_cast<const char*>(&ObjectsInThread), sizeof(ObjectsInThread));
    for (std::vector<std::string> v : gv) {
        add(v);
    }
}

void wmml::abcerr (std::string error) {
    std::cerr << "WMML_ERROR: " << error << std::endl;
    abort();
}
