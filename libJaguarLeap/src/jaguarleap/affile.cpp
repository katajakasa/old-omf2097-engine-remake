#include "jaguarleap/affile.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"
#include <cstring>

AFFile::~AFFile() {
    for(auto &a : this->anims) {
        delete a.anim;
    }
}

AFFile::AFFile(std::string filename) {
    Parser p(filename);

    // Read header
    p.data((char*)(&header), sizeof(AFHeader));

    // Read animations
    while((this->anim_n = p.ubyte()) < 70 && p.isok()) {
        // Animation structure
        AFAnimation a;
        a.id = this->anim_n;

        // Parse animation
        try {
            a.anim = new Animation(p);
        } catch(ParserException &ex) {
            throw ex;
        }

        // Read animation footer
        a.unknown = p.str(21);
        a.move_string = p.str(21);
        int footer_str_len = p.uword();
        a.footer_string = p.str(footer_str_len);

        // Save animation
        this->anims.push_back(a);
    }

    // Read footer
    p.data((char*)(&footer), sizeof(AFFooter));

    // Check if file is okay
    if(strcmp(footer.file_id,"FGHED") != 0) {
        throw ParserException("AF File not valid!");
    }
}

void AFFile::save(std::string filename) {
    Writer w;

    // Write header
    w.data((char*)(&header), sizeof(AFHeader));

    // Write animations
    for(auto &a : this->anims) {
        w.ubyte(a.id);
        a.anim->save(w);
        w.cstr(a.unknown, 21);
        w.cstr(a.move_string, 21);
        w.uword(a.footer_string.length());
        w.data(a.footer_string.c_str(), a.footer_string.length());
    }
    w.ubyte(this->anim_n);

    // Write footer
    w.data((char*)(&footer), sizeof(AFFooter));

    // Save file
    if(!w.save(filename)) {
        throw ParserException("Error while saving to file");
    }
}
