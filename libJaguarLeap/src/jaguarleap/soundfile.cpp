#include "jaguarleap/soundfile.h"
#include "jaguarleap/shared/parserexception.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/fileops/parser.h"
#include <cstring>

SoundFile::SoundFile(std::string filename) {
    Parser p(filename);

    // Find block count
    p.pass(4);
    uint32_t header_size = p.udword();
    int data_block_count = header_size / 4;

    // Read block sizes
    std::vector<uint32_t> data_block_offsets;
    for(int i = 0; i < data_block_count - 2; i++) {
        data_block_offsets.push_back(p.udword());
    }
    data_block_offsets.push_back(p.getfilesize());

    // Read blocks
    for(uint32_t off : data_block_offsets) {
        Sound s;
        s.len = off - p.getpos();
        s.data = 0;
        if(s.len <= 2) {
            s.len = 0;
            p.pass(2);
        } else {
            s.data = new char[s.len];
            p.data(s.data, s.len);
        }
        this->samples.push_back(s);
    }
}

SoundFile::~SoundFile() {
    for(Sound &s : this->samples) {
        if(s.len > 0) {
            delete[] s.data;
        }
    }
}

void SoundFile::save(std::string filename) {
    Writer w;
    uint32_t header_size = this->samples.size() * 4 + 4;

    // Write header
    w.udword(0);

    // Write header blocks
    uint32_t offset = header_size;
    for(auto &s : this->samples) {
        w.udword(offset);
        offset += (s.len == 0) ? 2 : s.len;
    }

    // Write data
    for(auto &s : this->samples) {
        if(s.len == 0) {
            w.uword(0);
        } else {
            w.data(s.data, s.len);
        }
    }

    // Save file
    if(!w.save(filename)) {
        throw ParserException("Error while saving to file");
    }
}

void SoundFile::replaceSound(unsigned int id, const char* data, int len) {
    if(id > this->samples.size()) {
        throw ParserException("ID Does not exist!");
    }
    Sound s = this->samples.at(id);
    if(s.len > 0) {
        delete[] s.data;
    }
    s.data = new char[len];
    memcpy(s.data, data, len);
    this->samples[id] = s;
}
