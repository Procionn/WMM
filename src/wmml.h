#ifndef WMML_H
#define WMML_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class wmml
{
private:
    unsigned short int ObjectsInThread = 0;
    std::filesystem::path filename;
    std::fstream targetFile;
public:
    wmml(std::filesystem::path file);
    wmml(std::filesystem::path file, int OIT);
    ~wmml();
    
    unsigned short int size();
    bool read(std::vector<std::string>& out);
    bool read();
    void add(std::vector<std::string>& in);
    void reset();
    int sizeRequest();
    void overwriting (int tag, int field, std::string newStr);
    bool skip(int counter);
    bool skip_sector(int counter); 
    std::string read_field();
    void replace(int& tag, std::vector<std::string>& in);
    void remove(int& tag);
private:
    void abcerr (std::string error);
};

#endif // WMML_H
