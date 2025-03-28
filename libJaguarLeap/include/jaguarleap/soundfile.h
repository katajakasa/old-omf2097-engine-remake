#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <string>
#include <vector>

typedef struct {
    uint32_t len;
    char *data;
} Sound;

class SoundFile {
public:
    std::vector<Sound> samples; // 299 in total

    SoundFile(std::string file);
    virtual ~SoundFile();
    void save(std::string filename);
    void replaceSound(unsigned int id, const char* data, int len);
};

#endif // SOUNDFILE_H
