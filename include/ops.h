#ifndef OPS_H
#define OPS_H

#include <vector>
#include <cstdint>

class BMP;

// Mirror padding
BMP MirrorPad(const BMP& bmp, int paddingSize);
void MirrorPadInplace(BMP& bmp, int paddingSize);

// Gamma / convolution / filters
BMP CorrectGamma(const BMP& bmp, double G);
void CorrectGammaInplace(BMP& bmp, double G);

BMP Conv(const BMP& bmp, const std::vector<std::vector<double>>& kernel);
void ConvInplace(BMP& bmp, const std::vector<std::vector<double>>& kernel);

BMP ApplyMedianFilter(const BMP& bmp, int filterSize);
void ApplyMedianFilterInplace(BMP& bmp, int filterSize);

BMP ApplyNlMeans(const BMP& bmp, float h, float hColor, int templateWindowSize, int searchWindowSize);
void ApplyNlMeansInplace(BMP& bmp, float h, float hColor, int templateWindowSize, int searchWindowSize);

// color operations
BMP CorrectColorTemperature(const BMP& bmp, bool mode);
void CorrectColorTemperatureInplace(BMP& bmp, bool mode);

BMP EnhanceImage(const BMP& bmp, double hueShift, double saturationFactor, double intensityFactor);
void EnhanceImageInplace(BMP& bmp, double hueShift, double saturationFactor, double intensityFactor);

BMP AdjustTemp(const BMP& bmp, double CbShift, double CrShift);
void AdjustTempInplace(BMP& bmp, double CbShift, double CrShift);

#endif

