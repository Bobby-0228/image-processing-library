#ifndef BMP_H
#define BMP_H

#include<vector>

class BMP
{
    private:
        #pragma pack(2)
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
        }header;

        std::vector<uint8_t> imageData;
        int height_, width_, rowSize_, colSize_, imageSize_, pixelCount_, bytesPerPixel_;

    public:
        BMP(std::string filename);
        void read(std::string filename);
        void write(std::string filename);
        int height() const;
        int width() const;
};

#endif