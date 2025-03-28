#include <sstream>
#include <dumb/dumb.h>

#include "engine.h"
#include "tools/exception.h"
#include "tools/log.h"

using namespace std;

/**
  * Well, it's main function. ... Yep.
  */
int main(int argc, char *argv[]) {
#ifndef _OMFDEBUG
    freopen("errorlog.txt","w",stdout);
#endif
    LOG("One Must Fall 2097 Remake Project (OMFG) Started!");

    // Set libdumb
    dumb_register_stdfiles();

    // Run engine
    try {
        LOG("Running engine.");
        Engine engine;
        engine.run();
    } catch(Exception &ex) {
        ex.print();
    }

    LOG("Graceful exit.");
    return 0;
}
