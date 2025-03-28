#pragma once

#include <sstream>
#include <string>
#include <fstream>
#include <map>

class IniReader {
private:
    std::string file;
    std::map<std::string, std::map<std::string, std::string> > conf;

    bool exists(std::string cat, std::string key);

public:
    IniReader(std::string file);
    IniReader() {}

    bool parse();

    int getInteger(std::string category, std::string key);
    float getFloat(std::string category, std::string key);
    bool getBoolean(std::string category, std::string key);
    std::string getString(std::string category, std::string key);
};
