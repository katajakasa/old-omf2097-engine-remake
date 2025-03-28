#include "language.h"
#include "tools/parser.h"
#include "tools/exception.h"
#include "tools/log.h"

using namespace std;

Language::Language(LangFile file) {
    switch(file) {
        case ENGLISH: this->file = "resources/ENGLISH.DAT"; break;
        case GERMAN: this->file = "resources/GERMAN.DAT"; break;
        default: THROW("Error; requested language file does not exist!");
    }
    if(!this->parse()) {
        THROW("Error; Parsing the requested language file failed!");
    }
    LOG("Language::Language(): Loaded " << this->stringblocks.size() << " strings from Language file.");
}

Language::~Language() {}

bool Language::parse() {
    Parser parser(this->file);

    // Read header blocks (uint32 nextpos, char[0x20] title)
    while(parser.isok() && parser.peek_udword() <= parser.getfilesize()) {
        StringBlock sb;
        sb.npos = parser.udword();
        sb.title = parser.cstr(0x20);
        this->stringblocks.push_back(sb);
    }

    unsigned char xorkey = 0;
    unsigned int len = 0;
    unsigned int npos = 0;
    for(unsigned int i = 0; i < this->stringblocks.size()-1 && parser.isok(); i++) {
        npos = this->stringblocks.at(i+1).npos; // Get next block position
        len = npos - parser.getpos(); // Length of this text block
        xorkey = len & 0xFF; // Xor seed

        // Read the string, character by character. Remember to XOR it.
        char r[len];
        for(unsigned int rp = 0; rp < len; rp++) {
            r[rp] = parser.rchar() ^ xorkey++;
        }
        this->stringblocks.at(i).text = std::string(r, len);
    }

    return true;
}
