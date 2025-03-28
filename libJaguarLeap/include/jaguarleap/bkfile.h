#ifndef BKFILE_H
#define BKFILE_H

#include <string>
#include <vector>
#include "jaguarleap/shared/animation.h"
#include "jaguarleap/shared/palette.h"

/** @brief BK-file header block.
 */
struct BKHeader {
    uint32_t id; /**< File ID */
    uint8_t unknown_a; /**< Unknown value */
    uint16_t img_w; /**< Background image width (always 320?) */
    uint16_t img_h; /**< Background image height (always 240?) */
};

/** @brief Structure for image data
 */
class BKImage {
public:
    char data[320*200]; /**< Background image data. Should always be 320x240 bytes (pixels) in size. */

    /** @brief Converts background image file to RGB image
     * @param output Output data, must be 320x200x3 bytes
     * @param palette Palette for the image
     */
    void convertToRGB(char* output, Palette &palette, int remap = -1);
};

/** @brief BK-file footer. Contains palettes & background image.
 */
struct BKFooter {
    BKImage img; /**< Background image */
    std::vector<Palette> palettes; /**< Palettes */
    char unknown[30]; /**< Unknown data (30 bytes) */
};

/** @brief BK-file specific animation structure (also contains generic animation).
 */
class BKAnimation {
public:
    uint8_t id;
    uint8_t unknown_a;
    uint8_t unknown_b;
    uint8_t unknown_c;
    uint8_t unknown_d;
    uint16_t unknown_e;
    uint8_t unknown_f;
    uint16_t unknown_g;
    std::string unknown_h;
    Animation *anim;
};

/** @brief BK-file parser
 */
class BKFile {
protected:
    int last_animno;

public:
    BKHeader header; /**< Headerblock */
    std::vector<BKAnimation> anims; /**< Animations */
    BKFooter footer; /**< Footerblock */

    BKFile(std::string file);
    virtual ~BKFile();
    void save(std::string filename);
};

#endif // BKFILE_H
