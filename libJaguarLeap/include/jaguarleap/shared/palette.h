#ifndef PALETTE_H
#define PALETTE_H

/** @brief Palette class, contains palette colors and remapping tables
 */
class Palette {
public:
    char data[256][3]; /**< Palette data */
    char remaps[19][256]; /**< Remapping data */
};

#endif // PALETTE_H
