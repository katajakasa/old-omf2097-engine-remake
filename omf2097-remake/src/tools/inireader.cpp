#include "inireader.h"
#include "tools/exception.h"

using namespace std;

IniReader::IniReader(std::string file) {
    this->file = file;
}

bool IniReader::parse() {
    ifstream in(this->file.c_str(), ifstream::in);

    // Check if opening file failed
    if(!in.good()) {
        return false;
    }

    string category;
    map<string, string> catmap;

    // Start reading line at a time
    char cline[128];
    while(in.good() && !in.eof()) {
        // Read new line
        in.getline(cline, 128);
        string line(cline);

        // Remove CR
        unsigned int cr = line.find_first_of('\r');
        if(cr != string::npos) {
            line = line.substr(0,cr-1);
        }

        // Stop at empty lines, comments
        if(line.length() == 0)
            continue;
        if(line.at(0) == '#')
            continue;

        // We find a category definition
        if(line.at(0) == '[') {
            // Save old category
            this->conf[category] = catmap;
            catmap.clear();

            // Init new category
            category = line.substr(1, line.find_first_of(']')-1);
            if(category.length() == 0)
                return false;
            continue;
        }

        // Read setting to map
        int eqpos = line.find_first_of("=");
        string key = line.substr(0, eqpos);
        string val = line.substr(eqpos+1);
        catmap[key] = val;
    }
    in.close();
    return true;
}

bool IniReader::exists(std::string cat, std::string key) {
    map<string, map<string, string> >::iterator cit;
    map<string, string>::iterator kit;

    // Check if category exits
    cit = this->conf.find(cat);
    if(cit != this->conf.end()) {

        // Check if key exists
        kit = this->conf[cat].find(key);
        if(kit != this->conf[cat].end()) {
            return true;
        }
    }

    return false;
}

std::string getDDString(std::string category, std::string key) {
    std::ostringstream o;
    o << "Category '" << category << "' and/or key '" << key << "' does not exist.";
    return o.str();
}

int IniReader::getInteger(std::string category, std::string key) {
    if(!this->exists(category, key)) {
        THROW(getDDString(category, key));
    }
    std::istringstream ss(this->conf[category][key]);

    int retval = 0;
    ss >> retval;
    if(ss.fail()) {
        THROW(std::string("Conversion failure (string to int)"));
    }
    return retval;
}

float IniReader::getFloat(std::string category, std::string key) {
    if(!this->exists(category, key)) {
        throw std::string(getDDString(category, key));
    }
    std::istringstream ss(this->conf[category][key]);

    float retval;
    ss >> retval;
    if(ss.fail()) {
        THROW(std::string("Conversion failure (string to float)"));
    }
    return retval;
}

bool IniReader::getBoolean(std::string category, std::string key) {
    if(!this->exists(category, key)) {
        THROW(getDDString(category, key));
    }

    // Try to parse string first
    if(this->conf[category][key].compare("true") == 0)
        return true;
    if(this->conf[category][key].compare("false") == 0)
        return false;

    // If no success, try automatic conversion
    std::istringstream ss(this->conf[category][key]);

    bool retval;
    ss >> retval;
    if(ss.fail()) {
        THROW(std::string("Conversion failure (string to bool)"));
    }
    return retval;
}

std::string IniReader::getString(std::string category, std::string key) {
    if(!this->exists(category, key)) {
        THROW(std::string(getDDString(category, key)));
    }
    return this->conf[category][key];
}
