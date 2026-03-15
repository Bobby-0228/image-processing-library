#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <vector>
#include <string>

class BMP
{
    friend void crop(BMP&, int, int, int, int);

private:
    #pragma pack(push, 1)
    struct Header
    {
        char signature[2];
        uint32_t fileSize;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t dataOffset;
        uint32_t headerSize;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bitsPerPixel;
        uint32_t compression;
        uint32_t imageSize;
        int32_t xPixelsPerMeter;
        int32_t yPixelsPerMeter;
        uint32_t colorsUsed;
        uint32_t colorsImportant;
    };
    #pragma pack(pop)

    Header header;
    std::vector<uint8_t> imageData_;
    int height_, width_, rowSize_, colSize_, imageSize_, pixelCount_, bytesPerPixel_;

public:
    using iterator = std::vector<uint8_t>::iterator;
    using const_iterator = std::vector<uint8_t>::const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    explicit BMP(std::string filename);

    void read(std::string filename);
    void write(std::string filename);
    void replaceRaw(std::vector<uint8_t>&& newData, int newWidth, int newHeight, int newRowSize);

    int height() const;
    int width() const;
    int bytesPerPixel() const;
    int rowSize() const;
    int pixelCount() const;

    uint8_t* data();
    const uint8_t* data() const;

    uint8_t* rowPtr(int y);
    const uint8_t* rowPtr(int y) const;

    uint8_t* pixelPtr(int x, int y);
    const uint8_t* pixelPtr(int x, int y) const;

    uint8_t& at(int x, int y, int c);
    const uint8_t& at(int x, int y, int c) const;

};

#endif

