#include "jaguarleap/languagefile.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/shared/parserexception.h"
#include <list>

LanguageFile::LanguageFile(std::string filename) {
    Parser p(filename);

    // Read header blocks to a list
    uint32_t offset;
    std::list<LanguageString> strings;
    while((offset = p.udword()) <= p.getfilesize()) {
        LanguageString s;
        s.offset = offset;
        s.title = p.cstr(0x20);
        strings.push_front(s);
    }

    // Read strings
    uint8_t xorkey = 0;
    int len = 0;
    int last_offset = p.getfilesize();
    for(auto &s : strings) {
        // Select the correct offset in file
        p.reset(s.offset);

        // Read the string
        len = last_offset - s.offset;
        xorkey = len & 0xFF;
        last_offset = s.offset;
        char tmp[len];

        // XOR string
        for(int i = 0; i < len; i++) {
            tmp[i] = p.rchar() ^ xorkey++;
        }

        // That's it, we've got the text
        s.text = std::string(tmp, len);
    }

    // Insert to vector for fast querying
    strings.reverse();
    for(auto &s : strings) {
        this->strs.push_back(s);
    }
}

void LanguageFile::save(std::string filename) {
    Writer header;
    Writer data;

    uint32_t offset = (0x20 + 4) * this->strs.size();
    uint8_t xorkey = 0;
    for(auto &s : this->strs) {
        // Write header
        header.udword(offset);
        header.cstr(s.title, 32);

        // Write data
        xorkey = s.text.length() & 0xFF;
        for(unsigned int i = 0; i < s.text.length(); i++) {
            data.ubyte(s.text[i] ^ xorkey++);
        }
        offset += s.text.length();
    }

    // Write to file
    Writer w;
    w.copy(header);
    w.copy(data);

    if(!w.save(filename)) {
        throw ParserException("Error while saving to file");
    }
}

std::string LanguageFile::getText(int id) {
    return this->strs.at(id).text;
}

std::string LanguageFile::getTitle(int id) {
    return this->strs.at(id).title;
}
