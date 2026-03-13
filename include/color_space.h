#ifndef COLOR_SPACE_H
#define COLOR_SPACE_H

#include <cstdint>

// HSI helpers
void RGB2HSI(uint8_t R, uint8_t G, uint8_t B, double& H, double& S, double& I);
void HSI2RGB(double H, double S, double I, uint8_t& R, uint8_t& G, uint8_t& B);

// YCbCr helpers
void RGB2YCbCr(uint8_t R, uint8_t G, uint8_t B, double& Y, double& Cb, double& Cr);
void YCbCr2RGB(double Y, double Cb, double Cr, uint8_t& R, uint8_t& G, uint8_t& B);

#endif

