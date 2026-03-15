#ifndef BASIC_H
#define BASIC_H

#include <cstdint>

class BMP;

uint8_t convertBit(uint8_t value, int bit);

// non-in-place versions
BMP quantize_resolution(const BMP& bmp, int targetBit);
BMP crop(const BMP& bmp, int x, int y, int w, int h);
BMP flip_horizontally(const BMP& bmp);
BMP flip_vertically(const BMP& bmp);

// in-place versions
void quantize_resolutionInplace(BMP& bmp, int targetBit);
void cropInplace(BMP& bmp, int x, int y, int w, int h);
void flip_horizontallyInplace(BMP& bmp);
void flip_verticallyInplace(BMP& bmp);

#endif

