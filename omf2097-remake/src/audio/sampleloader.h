#pragma once

#include <string>
#include <map>

class AudioSample;

class SampleLoader {
private:
    std::map<int, AudioSample*> samples;
    unsigned int iblocks, rblocks;
    void parse();

public:
    SampleLoader();
    ~SampleLoader();
    AudioSample* getSample(int id);
};
