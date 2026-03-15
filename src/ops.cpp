#include "ops.h"

#include "bmp.h"
#include "basic.h"
#include "color_space.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

BMP MirrorPad(const BMP& bmp, int paddingSize)
{
    BMP out = bmp;
    MirrorPadInplace(out, paddingSize);
    return out;
}

void MirrorPadInplace(BMP& bmp, int paddingSize)
{
    if (paddingSize <= 0)
    {
        std::cerr << "Padding size must be positive." << std::endl;
        return;
    }

    const int bytesPerPixel = bmp.bytesPerPixel();
    const int oldWidth = bmp.width();
    const int oldHeight = bmp.height();

    const int newWidth = oldWidth + 2 * paddingSize;
    const int newHeight = oldHeight + 2 * paddingSize;
    const int newRowSize = ((newWidth * bytesPerPixel + 3) & ~3);

    std::vector<uint8_t> paddedData(static_cast<size_t>(newRowSize) * newHeight, 0);

    auto copyPixel = [&](int srcX, int srcY, int dstX, int dstY)
    {
        const uint8_t* src = bmp.pixelPtr(srcX, srcY);
        uint8_t* dst = paddedData.data() + (dstY * newRowSize) + (dstX * bytesPerPixel);
        for (int c = 0; c < bytesPerPixel; ++c)
        {
            dst[c] = src[c];
        }
    };

    // center
    for (int y = 0; y < oldHeight; ++y)
    {
        for (int x = 0; x < oldWidth; ++x)
        {
            copyPixel(x, y, x + paddingSize, y + paddingSize);
        }
    }

    // mirror top/bottom (replicate closest row)
    for (int y = 0; y < paddingSize; ++y)
    {
        const int srcTopY = paddingSize;
        const int dstTopY = y;
        const int srcBottomY = newHeight - paddingSize - 1;
        const int dstBottomY = newHeight - 1 - y;

        for (int x = 0; x < newWidth; ++x)
        {
            const uint8_t* srcTop = paddedData.data() + (srcTopY * newRowSize) + (x * bytesPerPixel);
            uint8_t* dstTop = paddedData.data() + (dstTopY * newRowSize) + (x * bytesPerPixel);
            const uint8_t* srcBottom = paddedData.data() + (srcBottomY * newRowSize) + (x * bytesPerPixel);
            uint8_t* dstBottom = paddedData.data() + (dstBottomY * newRowSize) + (x * bytesPerPixel);

            for (int c = 0; c < bytesPerPixel; ++c)
            {
                dstTop[c] = srcTop[c];
                dstBottom[c] = srcBottom[c];
            }
        }
    }

    // mirror left/right (replicate closest col)
    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < paddingSize; ++x)
        {
            const int srcLeftX = paddingSize;
            const int dstLeftX = x;
            const int srcRightX = newWidth - paddingSize - 1;
            const int dstRightX = newWidth - 1 - x;

            const uint8_t* srcLeft = paddedData.data() + (y * newRowSize) + (srcLeftX * bytesPerPixel);
            uint8_t* dstLeft = paddedData.data() + (y * newRowSize) + (dstLeftX * bytesPerPixel);
            const uint8_t* srcRight = paddedData.data() + (y * newRowSize) + (srcRightX * bytesPerPixel);
            uint8_t* dstRight = paddedData.data() + (y * newRowSize) + (dstRightX * bytesPerPixel);

            for (int c = 0; c < bytesPerPixel; ++c)
            {
                dstLeft[c] = srcLeft[c];
                dstRight[c] = srcRight[c];
            }
        }
    }

    bmp.replaceRaw(std::move(paddedData), newWidth, newHeight, newRowSize);
}

BMP CorrectGamma(const BMP& bmp, double G)
{
    BMP out = bmp;
    CorrectGammaInplace(out, G);
    return out;
}

void CorrectGammaInplace(BMP& bmp, double G)
{
    for (auto& pixel : bmp)
    {
        pixel = static_cast<uint8_t>(std::pow((pixel / 255.0), 1.0 / G) * 255.0);
    }
}

BMP Conv(const BMP& bmp, const std::vector<std::vector<double>>& kernel)
{
    BMP out = bmp;
    ConvInplace(out, kernel);
    return out;
}

void ConvInplace(BMP& bmp, const std::vector<std::vector<double>>& kernel)
{
    const int kernelSize = static_cast<int>(kernel.size());
    if (kernelSize == 0 || kernelSize % 2 == 0 || kernelSize != static_cast<int>(kernel[0].size()))
    {
        std::cerr << "Kernel must be a square matrix with odd dimensions." << std::endl;
        return;
    }

    const int kernelRadius = kernelSize / 2;
    const int bytesPerPixel = bmp.bytesPerPixel();
    const int rowSize = bmp.rowSize();
    const int width = bmp.width();
    const int height = bmp.height();

    std::vector<uint8_t> originalData(bmp.data(), bmp.data() + (rowSize * height));

    for (int y = kernelRadius; y < height - kernelRadius; ++y)
    {
        for (int x = kernelRadius; x < width - kernelRadius; ++x)
        {
            double sum[4] = {0.0, 0.0, 0.0, 0.0};

            for (int ky = -kernelRadius; ky <= kernelRadius; ++ky)
            {
                for (int kx = -kernelRadius; kx <= kernelRadius; ++kx)
                {
                    const int px = x + kx;
                    const int py = y + ky;
                    const int idx = (py * rowSize) + (px * bytesPerPixel);

                    for (int c = 0; c < bytesPerPixel; ++c)
                    {
                        sum[c] += originalData[idx + c] * kernel[ky + kernelRadius][kx + kernelRadius];
                    }
                }
            }

            uint8_t* dst = bmp.pixelPtr(x, y);
            for (int c = 0; c < bytesPerPixel; ++c)
            {
                dst[c] = static_cast<uint8_t>(std::clamp(static_cast<int>(std::lround(sum[c])), 0, 255));
            }
        }
    }
}

BMP ApplyMedianFilter(const BMP& bmp, int filterSize)
{
    BMP out = bmp;
    ApplyMedianFilterInplace(out, filterSize);
    return out;
}

void ApplyMedianFilterInplace(BMP& bmp, int filterSize)
{
    if (filterSize % 2 == 0 || filterSize < 3)
    {
        std::cerr << "Filter size must be an odd number >= 3." << std::endl;
        return;
    }

    const int radius = filterSize / 2;
    BMP padded = MirrorPad(bmp, radius);

    const int bytesPerPixel = bmp.bytesPerPixel();
    const int width = bmp.width();
    const int height = bmp.height();

    const int paddedRowSize = padded.rowSize();
    const int paddedWidth = padded.width();
    (void)paddedWidth;

    std::vector<uint8_t> outData(static_cast<size_t>(bmp.rowSize()) * height, 0);

    const int windowArea = filterSize * filterSize;
    std::vector<uint8_t> channel;
    channel.reserve(windowArea);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            for (int c = 0; c < bytesPerPixel; ++c)
            {
                channel.clear();

                for (int dy = -radius; dy <= radius; ++dy)
                {
                    for (int dx = -radius; dx <= radius; ++dx)
                    {
                        const int px = x + radius + dx;
                        const int py = y + radius + dy;
                        const uint8_t* p = padded.data() + (py * paddedRowSize) + (px * bytesPerPixel);
                        channel.push_back(p[c]);
                    }
                }

                // find the median value
                std::nth_element(channel.begin(), channel.begin() + channel.size() / 2, channel.end());
                const uint8_t med = channel[channel.size() / 2];

                outData[(y * bmp.rowSize()) + (x * bytesPerPixel) + c] = med;
            }
        }
    }

    bmp.replaceRaw(std::move(outData), width, height, bmp.rowSize());
}

BMP FastNlMeansDenoise(const BMP& bmp, float h, float hColor, int templateWindowSize, int searchWindowSize)
{
    BMP out = bmp;
    FastNlMeansDenoiseInplace(out, h, hColor, templateWindowSize, searchWindowSize);
    return out;
}

void FastNlMeansDenoiseInplace(BMP& bmp, float h, float hColor, int templateWindowSize, int searchWindowSize)
{

    if (searchWindowSize % 2 == 0) searchWindowSize += 1;
    if (templateWindowSize % 2 == 0) templateWindowSize += 1;
    if (h <= 0 || hColor <= 0)
    {
        std::cerr << "Invalid h or hColor parameter; both must be > 0." << std::endl;
        return;
    }

    const int bytesPerPixel = bmp.bytesPerPixel();
    if (bytesPerPixel < 3)
    {
        std::cerr << "FastNlMeansDenoise expects at least 3 channels." << std::endl;
        return;
    }

    const int width = bmp.width();
    const int height = bmp.height();
    const int rowSize = bmp.rowSize();

    const int patchRadius = templateWindowSize / 2;
    const int searchRadius = searchWindowSize / 2;

    std::vector<uint8_t> denoisedData(static_cast<size_t>(rowSize) * height, 0);

    auto luminanceAt = [&](int x, int y)
    {
        const uint8_t* p = bmp.pixelPtr(x, y);
        const double b = p[0], g = p[1], r = p[2];
        return 0.2989 * r + 0.5870 * g + 0.1140 * b;
    };

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double sumWeights[3] = {0.0, 0.0, 0.0};
            double sumWeightedValues[3] = {0.0, 0.0, 0.0};

            const double lumCenter = luminanceAt(x, y);

            for (int dy = -searchRadius; dy <= searchRadius; ++dy)
            {
                for (int dx = -searchRadius; dx <= searchRadius; ++dx)
                {
                    const int nx = x + dx;
                    const int ny = y + dy;
                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;

                    const double lumNeighbor = luminanceAt(nx, ny);

                    double patchDistanceLum = 0.0;
                    double patchDistanceColor[3] = {0.0, 0.0, 0.0};

                    for (int py = -patchRadius; py <= patchRadius; ++py)
                    {
                        for (int px = -patchRadius; px <= patchRadius; ++px)
                        {
                            const int cx = std::clamp(x + px, 0, width - 1);
                            const int cy = std::clamp(y + py, 0, height - 1);
                            const int nnx = std::clamp(nx + px, 0, width - 1);
                            const int nny = std::clamp(ny + py, 0, height - 1);

                            const uint8_t* pc = bmp.pixelPtr(cx, cy);
                            const uint8_t* pn = bmp.pixelPtr(nnx, nny);

                            for (int c = 0; c < 3; ++c)
                            {
                                const double diff = static_cast<double>(pc[c]) - static_cast<double>(pn[c]);
                                patchDistanceColor[c] += (diff * diff) / (255.0 * 255.0);
                            }

                            const double diffLum = lumCenter - lumNeighbor;
                            patchDistanceLum += (diffLum * diffLum) / (255.0 * 255.0);
                        }
                    }

                    const double weightLum = std::exp(-patchDistanceLum / (h * h));
                    double weightColor[3];
                    for (int c = 0; c < 3; ++c)
                    {
                        weightColor[c] = std::exp(-patchDistanceColor[c] / (hColor * hColor));
                    }

                    const uint8_t* neighborPix = bmp.pixelPtr(nx, ny);
                    for (int c = 0; c < 3; ++c)
                    {
                        const double w = weightLum * weightColor[c];
                        sumWeightedValues[c] += w * neighborPix[c];
                        sumWeights[c] += w;
                    }
                }
            }

            uint8_t* outPix = denoisedData.data() + (y * rowSize) + (x * bytesPerPixel);
            for (int c = 0; c < 3; ++c)
            {
                outPix[c] = static_cast<uint8_t>(
                    std::min(255.0, sumWeightedValues[c] / (sumWeights[c] + 1e-5))
                );
            }
            // copy alpha if present
            if (bytesPerPixel == 4)
            {
                outPix[3] = bmp.pixelPtr(x, y)[3];
            }
        }
    }

    bmp.replaceRaw(std::move(denoisedData), width, height, rowSize);
}

BMP CorrectColorTemperature(const BMP& bmp, bool mode)
{
    BMP out = bmp;
    CorrectColorTemperatureInplace(out, mode);
    return out;
}

void CorrectColorTemperatureInplace(BMP& bmp, bool mode)
{
    const int width = bmp.width();
    const int height = bmp.height();
    const int bytesPerPixel = bmp.bytesPerPixel();

    if (bytesPerPixel < 3)
    {
        std::cerr << "CorrectColorTemperature expects at least 3 channels." << std::endl;
        return;
    }

    const int pixelCount = width * height;

    double scaleR = 1.0, scaleG = 1.0, scaleB = 1.0;
    if (mode)
    {
        uint8_t maxR = 0, maxG = 0, maxB = 0;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const uint8_t* p = bmp.pixelPtr(x, y);
                maxB = std::max(maxB, p[0]);
                maxG = std::max(maxG, p[1]);
                maxR = std::max(maxR, p[2]);
            }
        }
        scaleR = 255.0 / std::max<uint8_t>(1, maxR);
        scaleG = 255.0 / std::max<uint8_t>(1, maxG);
        scaleB = 255.0 / std::max<uint8_t>(1, maxB);
    }
    else
    {
        double sumR = 0.0, sumG = 0.0, sumB = 0.0;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const uint8_t* p = bmp.pixelPtr(x, y);
                sumB += p[0];
                sumG += p[1];
                sumR += p[2];
            }
        }
        const double avgR = sumR / pixelCount;
        const double avgG = sumG / pixelCount;
        const double avgB = sumB / pixelCount;
        const double grayValue = (avgR + avgG + avgB) / 3.0;
        scaleR = grayValue / (avgR + 1e-10);
        scaleG = grayValue / (avgG + 1e-10);
        scaleB = grayValue / (avgB + 1e-10);
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uint8_t* p = bmp.pixelPtr(x, y);
            p[0] = static_cast<uint8_t>(std::min(255.0, p[0] * scaleB));
            p[1] = static_cast<uint8_t>(std::min(255.0, p[1] * scaleG));
            p[2] = static_cast<uint8_t>(std::min(255.0, p[2] * scaleR));
        }
    }
}

BMP EnhanceImage(const BMP& bmp, double hueShift, double saturationFactor, double intensityFactor)
{
    BMP out = bmp;
    EnhanceImageInplace(out, hueShift, saturationFactor, intensityFactor);
    return out;
}

void EnhanceImageInplace(BMP& bmp, double hueShift, double saturationFactor, double intensityFactor)
{
    const int width = bmp.width();
    const int height = bmp.height();
    const int bytesPerPixel = bmp.bytesPerPixel();
    if (bytesPerPixel < 3)
    {
        std::cerr << "EnhanceImage expects at least 3 channels." << std::endl;
        return;
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uint8_t* p = bmp.pixelPtr(x, y);
            uint8_t R = p[2], G = p[1], B = p[0];

            double H, S, I;
            RGB2HSI(R, G, B, H, S, I);

            H += hueShift;
            if (H >= 2.0 * M_PI) H -= 2.0 * M_PI;
            if (H < 0.0) H += 2.0 * M_PI;

            S *= saturationFactor;
            I *= intensityFactor;

            HSI2RGB(H, S, I, R, G, B);
            p[2] = R;
            p[1] = G;
            p[0] = B;
        }
    }
}

BMP AdjustTemp(const BMP& bmp, double CbShift, double CrShift)
{
    BMP out = bmp;
    AdjustTempInplace(out, CbShift, CrShift);
    return out;
}

void AdjustTempInplace(BMP& bmp, double CbShift, double CrShift)
{
    const int width = bmp.width();
    const int height = bmp.height();
    const int bytesPerPixel = bmp.bytesPerPixel();
    if (bytesPerPixel < 3)
    {
        std::cerr << "AdjustTemp expects at least 3 channels." << std::endl;
        return;
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uint8_t* p = bmp.pixelPtr(x, y);
            uint8_t R = p[2], G = p[1], B = p[0];

            double Y, Cb, Cr;
            RGB2YCbCr(R, G, B, Y, Cb, Cr);

            Cb = std::max(0.0, std::min(255.0, Cb + CbShift));
            Cr = std::max(0.0, std::min(255.0, Cr + CrShift));

            YCbCr2RGB(Y, Cb, Cr, R, G, B);
            p[2] = R;
            p[1] = G;
            p[0] = B;
        }
    }
}

