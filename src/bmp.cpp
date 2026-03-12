#include "bmp.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

BMP::BMP(std::string filename)
{
    read(std::move(filename));
}

void BMP::read(std::string filename)
{
    std::ifstream bmpFile(filename, std::ios::binary);
    if (!bmpFile)
    {
        std::cerr << "Failed to open input BMP file." << std::endl;
        return;
    }

    bmpFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    // check the input format
    if (header.signature[0] != 'B' || header.signature[1] != 'M')
    {
        std::cerr << "The input file is not a BMP file." << std::endl;
        return;
    }

    if (header.bitsPerPixel != 24 && header.bitsPerPixel != 32)
    {
        std::cerr << "The input file is not a 24bits or 32bits file!" << std::endl;
        return;
    }

    // update members
    height_ = static_cast<int>(header.height);
    width_ = static_cast<int>(header.width);
    bytesPerPixel_ = static_cast<int>(header.bitsPerPixel) / 8;
    rowSize_ = ((width_ * bytesPerPixel_ + 3) & ~3);
    imageSize_ = header.imageSize == 0 ? rowSize_ * std::abs(height_) : header.imageSize;
    pixelCount_ = width_ * height_;

    // store the image data
    imageData_.resize(imageSize_);
    bmpFile.read(reinterpret_cast<char*>(imageData_.data()), imageSize_);
}

void BMP::write(std::string filename)
{
    std::ofstream outputBmpFile(filename, std::ios::binary);
    outputBmpFile.write(reinterpret_cast<char*>(&header), sizeof(header));
    outputBmpFile.write(reinterpret_cast<char*>(imageData_.data()), imageSize_);
    std::cout << "BMP file [" << filename << "] was written successfully." << std::endl;
}

BMP::iterator BMP::begin()
{
    return imageData_.begin();
}

BMP::iterator BMP::end()
{
    return imageData_.end();
}

BMP::const_iterator BMP::begin() const
{
    return imageData_.begin();
}

BMP::const_iterator BMP::end() const
{
    return imageData_.end();
}

int BMP::height() const
{
    return header.height;
}

int BMP::width() const
{
    return header.width;
}

int BMP::bytesPerPixel() const
{
    return bytesPerPixel_;
}

int BMP::rowSize() const
{
    return rowSize_;
}

int BMP::pixelCount() const
{
    return pixelCount_;
}

uint8_t* BMP::data()
{
    return imageData_.data();
}

const uint8_t* BMP::data() const
{
    return imageData_.data();
}

uint8_t* BMP::rowPtr(int y)
{
    return imageData_.data() + y * rowSize_;
}

const uint8_t* BMP::rowPtr(int y) const
{
    return imageData_.data() + y * rowSize_;
}

uint8_t* BMP::pixelPtr(int x, int y)
{
    return rowPtr(y) + x * bytesPerPixel_;
}

const uint8_t* BMP::pixelPtr(int x, int y) const
{
    return rowPtr(y) + x * bytesPerPixel_;
}

uint8_t& BMP::at(int x, int y, int c)
{
    return *(pixelPtr(x, y) + c);
}

const uint8_t& BMP::at(int x, int y, int c) const
{
    return *(pixelPtr(x, y) + c);
}

