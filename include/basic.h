#ifndef BASIC_H
#define BASIC_H

#include <cstdint>

class BMP;

uint8_t convertBit(uint8_t value, int bit);

BMP quantize_resolution(BMP bmp, int targetBit);

void crop(BMP& bmp, int x, int y, int w, int h);
void flip_horizontally(BMP& bmp);
BMP flip_vertically(const BMP& bmp);

#endif // BASIC_H

