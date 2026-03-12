#include<iostream>
#include<fstream>
#include<vector>

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
    }header;
    #pragma pack(pop)

    std::vector<uint8_t> imageData_;
    int height_, width_, rowSize_, colSize_, imageSize_, pixelCount_, bytesPerPixel_;

public:
    
    BMP(std::string filename)
    {
        read(filename);
    }

    void read(std::string filename)
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
        bytesPerPixel_ = static_cast<int>(header.bitsPerPixel) / 8;
        rowSize_ = ((width_ * bytesPerPixel_ + 3) & ~3);
        imageSize_ = header.imageSize == 0 ? rowSize_ * std::abs(height_) : header.imageSize;
        pixelCount_ = width_ * height_;
        //store the image data
        imageData_.resize(imageSize_);
        bmpFile.read(reinterpret_cast<char*>(imageData_.data()), imageSize_);
    }

    void write(std::string filename)
    {
        std::ofstream outputBmpFile(filename , std::ios::binary);
        outputBmpFile.write(reinterpret_cast<char*>(&header), sizeof(header));
        outputBmpFile.write(reinterpret_cast<char*>(imageData_.data()), imageSize_);
        std::cout << "BMP file [" << filename << "] was written successfully." << std::endl;
    }

    void pixel

    int height() const
    {
        return header.height;
    }

    int width() const
    {
        return header.width;
    }

    int bytesPerPixel() const
    {
        return bytesPerPixel_;
    }

    int rowSize() const
    {
        return rowSize_;
    }

    int pixelCount() const
    {
        return pixelCount_;
    }

    uint8_t* data()
    {
        return imageData_.data();
    }

    const uint8_t* data() const
    {
        return imageData_.data();
    }

    uint8_t* rowPtr(int y)
    {
        return imageData_.data() + y * rowSize_;
    }

    const uint8_t* rowPtr(int y) const
    {
        return imageData_.data() + y * rowSize_;
    }

    uint8_t* pixelPtr(int x, int y)
    {
        return rowPtr(y) + x * bytesPerPixel_;
    }

    const uint8_t* pixelPtr(int x, int y) const
    {
        return rowPtr(y) + x * bytesPerPixel_;
    }

    uint8_t& at(int x, int y, int c)
    {
        return *(pixelPtr(x, y) + c);
    }

    const uint8_t& at(int x, int y, int c) const
    {
        return *(pixelPtr(x, y) + c);
    }

};

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

BMP flip_horizontally(const BMP& bmp)
{
    BMP out = bmp;
    for (int y = 0; y < out.height(); y++)
    {
        for (int x = 0; x < out.width() / 2; x++)
        {
            uint8_t* left  = img.pixelPtr(x, y);
            uint8_t* right = img.pixelPtr(img.width()-1-x, y);

            for (int c = 0; c < out.bytesPerPixel(); c++)
            {
                //swap the pixel 
                std::swap(left[c], right[c]);
            }
        }
    }
}


int main()
{
    BMP bmp("images/inputs/normal/input1.bmp");
    crop(bmp, 180, 290, 300, 300);
    bmp.write("output.bmp");
    return 0;
}
