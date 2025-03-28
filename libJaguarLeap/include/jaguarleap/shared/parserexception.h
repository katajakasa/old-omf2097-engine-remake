#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <string>

/** @brief Default exception for the whole library.
 */
class ParserException {
protected:
    std::string error;

public:
    /** @brief Initializes the exception with an error message
     *
     * @param error - Error string
     */
    ParserException(std::string error) { this->error = error; }

    /** @brief Gets the error string from the exception
     *
     * @return Error string
     */
    std::string getError() { return this->error; }
};

#endif // PARSEREXCEPTION_H
