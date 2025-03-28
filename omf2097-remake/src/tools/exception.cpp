#include "exception.h"

#include <iostream>

using namespace std;

Exception::Exception(const Exception &ex) {
    this->msgs = ex.msgs;
}

Exception::Exception() {}

void Exception::add_message(int line, std::string file, std::string function, std::string message) {
    Message m;
    m.line = line;
    m.file = file;
    m.function = function;
    m.message = message;
    this->msgs.push_back(m);
}

void Exception::print() {
    int number = 1;
    list<Message>::iterator it;
    cout << "Exception trace:" << endl;
    for(it = msgs.begin(); it != msgs.end(); it++) {
        Message m = (*it);
        cout << "  " << number++ << ". Line " << m.line << " in file " << m.file << ", in function '" << m.function << "'";
        if(m.message.size() > 0) {
            cout << " with message: '" << m.message << "'";
        }
        cout << "." << endl;
    }
}
