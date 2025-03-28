#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
#include <vector>

class Language {
private:
    bool parse();

    std::string file;

public:
    enum LangFile {
        ENGLISH = 0,
        GERMAN
    };

    class StringBlock {
    public:
        unsigned int npos;
        std::string title;
        std::string text;
    };

    std::vector<StringBlock> stringblocks;

    Language(LangFile file);
    virtual ~Language();
};

#endif // LANGUAGE_H
