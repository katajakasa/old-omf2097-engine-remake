#include "sampleloader.h"
#include "tools/parser.h"
#include "audiosample.h"
#include "tools/exception.h"
#include "tools/log.h"

#include <vector>

using namespace std;

SampleLoader::SampleLoader() {
    this->iblocks = this->rblocks = 0;
    this->parse();
    LOG("Found " << iblocks << " blocks of audio, of which " << rblocks << " contain real data.");
}

SampleLoader::~SampleLoader() {
    map<int, AudioSample*>::iterator it;
    for(it = samples.begin(); it != samples.end(); it++) {
        delete (*it).second;
    }
    LOG("Closed.");
}

void SampleLoader::parse() {
    Parser *parser;
    try {
        parser = new Parser("resources/SOUNDS.DAT");
    } catch(Exception &ex) {
        RETHROW(ex);
    }

    parser->pass(4);
    unsigned int size = parser->udword();
    this->iblocks = size/4;
    vector<unsigned int> sizes;
    sizes.push_back(size);

    for(unsigned int i = 0; i < iblocks-2; i++) {
        unsigned int a = parser->udword();
        sizes.push_back(a);
    }

    for(unsigned int i = 0; i < sizes.size()-1; i++) {
        unsigned int read = sizes.at(i+1) - sizes.at(i);

        // Read sample
        if(read > 2) {
            this->rblocks++;

            // Read sound data, and convert to signed 16bit
            int16_t output[read-2];
            parser->pass(2);
            for(unsigned int ig = 0; ig < read-2; ig++) {
                output[ig] = (parser->ubyte() - 128) * 255;
            }

            // Create sample
            AudioSample *sample = 0;
            try {
                sample = new AudioSample(output, read-2, 8000, 1);
            } catch(Exception &ex) {
                RETHROW(ex);
            }

            // Add sample to list
            this->samples[i] = sample;
         } else {
            /* If sample is \0\0, just pass. No real data. */
            parser->pass(read);
        }
    }

    // Some debugging stuff, delete parser
    delete parser;
}

AudioSample* SampleLoader::getSample(int id) {
    return samples.at(id);
}
