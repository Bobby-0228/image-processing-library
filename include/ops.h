#ifndef OPS_H
#define OPS_H

#include <vector>
#include <cstdint>

class BMP;

BMP MirrorPad(const BMP& bmp, int paddingSize);
void CorrectGamma(BMP& bmp, double G);
void Conv(BMP& bmp, const std::vector<std::vector<double>>& kernel);
void ApplyMedianFilter(BMP& bmp, int filterSize);
void FastNlMeansDenoise(BMP& bmp, float h, float hColor, int templateWindowSize, int searchWindowSize);

// color operations
void CorrectColorTemperature(BMP& bmp, bool mode);
void EnhanceImage(BMP& bmp, double hueShift, double saturationFactor, double intensityFactor);
void AdjustTemp(BMP& bmp, double CbShift, double CrShift);

#endif

