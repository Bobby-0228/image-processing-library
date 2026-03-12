#include "basic.h"
#include "bmp.h"

#include <vector>
#include <algorithm>
#include <iostream>

uint8_t convertBit(uint8_t value, int bit)
{
    return ((value >> (8 - bit)) << (8 - bit));
}

BMP quantize_resolution(BMP bmp, int targetBit)
{
    for (auto &pixel : bmp)
    {
        pixel = convertBit(pixel, targetBit);
    }
    return bmp;
}

void crop(BMP& bmp, int x, int y, int w, int h)
{
    // Validate the input crop area
    if (x < 0 || y < 0 || x + w > bmp.header.width || y + h > bmp.header.height)
    {
        std::cerr << "Invalid crop dimensions." << std::endl;
        return;
    }

    // New cropped image data
    std::vector<uint8_t> croppedData;
    int newRowSize = ((w * bmp.bytesPerPixel_ + 3) & ~3); // Padded row size for cropped image
    croppedData.resize(newRowSize * h);

    // Copy pixel data from original image to the cropped area
    for (int i = 0; i < h; i++)  // Loop over the height of the cropped area
    {
        int srcRowStart = (y + i) * bmp.rowSize_ + x * bmp.bytesPerPixel_;  // Source row starting index in the original data
        int destRowStart = i * newRowSize;  // Destination row starting index in the cropped data

        std::copy(bmp.imageData_.begin() + srcRowStart,
                  bmp.imageData_.begin() + srcRowStart + w * bmp.bytesPerPixel_,
                  croppedData.begin() + destRowStart);
    }

    // Replace the imageData with croppedData
    bmp.imageData_ = std::move(croppedData);

    // Update the header to reflect the cropped size
    bmp.header.width = w;
    bmp.header.height = h;

    // Recalculate row size and image size for the cropped image
    bmp.rowSize_ = newRowSize;
    bmp.imageSize_ = newRowSize * h;
    bmp.header.imageSize = bmp.imageSize_;
}

void flip_horizontally(BMP& bmp)
{
    for (int y = 0; y < bmp.height(); y++)
    {
        for (int x = 0; x < bmp.width() / 2; x++)
        {
            uint8_t* left  = bmp.pixelPtr(x, y);
            uint8_t* right = bmp.pixelPtr(bmp.width() - 1 - x, y);

            for (int c = 0; c < bmp.bytesPerPixel(); c++)
            {
                //swap the pixel 
                std::swap(left[c], right[c]);
            }
        }
    }
}

BMP flip_vertically(const BMP& bmp)
{
    BMP out = bmp;
    for (int y = 0; y < out.height() / 2; y++)
    {
        for (int x = 0; x < out.width(); x++)
        {
            uint8_t* top = out.pixelPtr(x, y);
            uint8_t* bottom = out.pixelPtr(x, out.height() - 1 - y);
            for (int c = 0; c < out.bytesPerPixel(); c++)
            {
                std::swap(top[c], bottom[c]);
            }
        }
    }
    return out;
}

