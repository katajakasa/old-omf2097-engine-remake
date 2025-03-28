#include "har.h"

#include "tools/parser.h"
#include "tools/exception.h"
#include "tools/log.h"

using namespace std;

Har::Har(string file) {
    this->file = file;
    if(!this->parse()) {
        THROW("Error while trying to parse HAR " << file << "!");
    }
}

Har::~Har() {
    // Delete textures here, if parse has been run.
}

bool Har::parse() {
    // Open parsers
    Parser parser(file.c_str());
    if(!parser.isok()) {
        return false;
    }

    // Load Fighter Header
    this->robot_number = parser.word();
    parser.pass(3);
    this->endurance = parser.int3();
    parser.pass(1);
    this->power = parser.word();
    this->forward_mov = parser.dword();
    this->backward_mov = parser.dword();
    this->up_mov = parser.dword();
    this->down_mov = parser.dword();
    parser.pass(2);

    // Start reading animations
    while(((uint8_t)parser.peek_byte()) != 0xFA) {
        Animation *anim = new Animation;

        // Load Animation Header
        anim->anim_no = parser.ubyte();

        // Parse generic animation
        anim->parse(&parser, &this->palette);

        // Load Animation footer
        parser.data(anim->unknown_footer_data, 21);
        parser.data(anim->movement_string, 21);
        int extra_footer_string_len = parser.word();
        anim->extra_footer_string = parser.str(extra_footer_string_len);

        this->anims.push_back(anim);
    }
    return true;
}

Animation* Har::seek_animation(int anim_id) {
    for(unsigned int i = 0; i < anims.size(); i++) {
        if(anims.at(i)->anim_no == anim_id) {
            return anims.at(i);
        }
    }
    return 0;
}

Palette* Har::get_palette(int number) {
    return 0;
}

void Har::printDebug() {
    LOG("[Har]");
    LOG("  File: " << this->file);
    LOG("  Number: " << this->robot_number);
    LOG("  Endurance: " << this->endurance);
    LOG("  Power: " << this->power );
    LOG("  Movement: (Fwd, Bwd, Up, Dwn)=(" << this->forward_mov << ", " << this->backward_mov << "," << this->up_mov << "," << this->down_mov << ")");
    LOG("  Animations (" << this->anims.size() << "): ");
    for(unsigned int i = 0; i < this->anims.size(); i++) {
        this->anims.at(i)->printDebug();
    }
    LOG("[/Har]");
}
