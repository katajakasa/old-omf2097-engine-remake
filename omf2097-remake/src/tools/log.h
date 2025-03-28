#ifndef LOG_H
#define LOG_H


#ifdef _OMFDEBUG
#include <iostream>
#define LOG(message) std::cout << __FUNCTION__ << ": " << message << std::endl
#else
#define LOG(message)
#endif

#endif // LOG_H
