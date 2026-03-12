#include "bmp.h"

BMP::BMP(std::string filename)
{
    read(filename);
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

    //check the input format
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

    //update members
    height_ = static_cast<int>(header.height);
    width_ = static_cast<int>(header.width);
    bytesPerPixel = static_cast<int>(header.bitsPerPixel) / 8;
    rowSize_ = ((width_ * bytesPerPixel + 3) & ~3);
    imageSize_ = header.imageSize == 0 ? header.width * header.height * bytesPerPixel : header.imageSize;
    pixelCount = header.width * header.height;
    //store the image data
    imageData.resize(imageSize);
    bmpFile.read(reinterpret_cast<char*>(imageData.data()), imageSize);
}

void BMP::write(std::string filename)
{
    std::ofstream outputBmpFile(filename , std::ios::binary);
    outputBmpFile.write(reinterpret_cast<char*>(&header), sizeof(header));
    outputBmpFile.write(reinterpret_cast<char*>(imageData.data()), imageSize);
    std::cout << "BMP file [" << filename << "] was written successfully." << std::endl;
}

int BMP::height() const
{
    return header.height;
}

int BMP::width() const
{
    return header.width;
}