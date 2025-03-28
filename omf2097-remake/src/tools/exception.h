#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <list>
#include <sstream>

#define THROW(message) \
    {\
        std::ostringstream __o; \
        __o << message; \
        Exception __e; \
        __e.add_message(__LINE__, __FILE__, __PRETTY_FUNCTION__, __o.str()); \
        throw __e; \
    }

#define RETHROW(ex) \
    {\
        Exception __e(ex);\
        __e.add_message(__LINE__, __FILE__, __PRETTY_FUNCTION__, "");\
        throw __e;\
    }

class Exception {
public:
    // Internal stuff, really
    class Message {
    public:
        int line;
        std::string file;
        std::string function;
        std::string message;
    };
    std::list<Message> msgs;

    // api
    Exception(const Exception &ex);
    Exception();
    void add_message(int line, std::string file, std::string function, std::string message);
    void print();
};

#endif // EXCEPTION_H
