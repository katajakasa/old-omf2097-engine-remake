#ifndef LANGUAGEFILE_H
#define LANGUAGEFILE_H

#include <string>
#include <vector>

struct LanguageString {
    uint32_t offset;
    std::string title;
    std::string text;
};

/** @brief A class for parsing and saving OMF language files.
 */
class LanguageFile {
protected:
    std::vector<LanguageString> strs;

public:
    /** @brief Opens the file and parses the contents
     *
     * @param filename - Name of the file to parse
     * @throw ParserException
     */
    LanguageFile(std::string filename);

    /** @brief Saves the data to a file
     *
     * @param filename - Name of the file to save to
     * @throw ParserException
     */
    void save(std::string filename);

    /** @brief Returns the text with given id
     *
     * @param id - ID of the wanted text string
     */
    std::string getText(int id);

    /** @brief Returns the title with given id
     *
     * @param id - ID of the wanted title string
     */
    std::string getTitle(int id);
};

#endif // LANGUAGEFILE_H
