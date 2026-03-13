#include "color_space.h"

#include <algorithm>
#include <cmath>

void RGB2HSI(uint8_t R, uint8_t G, uint8_t B, double& H, double& S, double& I)
{
    double r = R / 255.0;
    double g = G / 255.0;
    double b = B / 255.0;

    double num = 0.5 * ((r - g) + (r - b));
    double den = std::sqrt((r - g) * (r - g) + (r - b) * (g - b));
    double theta = std::acos(num / (den + 1e-10));
    if (b <= g)
        H = theta;
    else
        H = 2.0 * M_PI - theta;

    double min_RGB = std::min(std::min(b, g), r);
    double sum = r + g + b;
    S = 1.0 - 3.0 * min_RGB / (sum + 1e-10);
    I = sum / 3.0;
}

void HSI2RGB(double H, double S, double I, uint8_t& R, uint8_t& G, uint8_t& B)
{
    double r, g, b;
    if (H >= 0 && H < 2.0 * M_PI / 3.0)
    {
        b = I * (1.0 - S);
        r = I * (1.0 + S * std::cos(H) / std::cos(M_PI / 3.0 - H));
        g = 3.0 * I - (r + b);
    }
    else if (H >= 2.0 * M_PI / 3.0 && H < 4.0 * M_PI / 3.0)
    {
        H -= 2.0 * M_PI / 3.0;
        r = I * (1.0 - S);
        g = I * (1.0 + S * std::cos(H) / std::cos(M_PI / 3.0 - H));
        b = 3.0 * I - (r + g);
    }
    else
    {
        H -= 4.0 * M_PI / 3.0;
        g = I * (1.0 - S);
        b = I * (1.0 + S * std::cos(H) / std::cos(M_PI / 3.0 - H));
        r = 3.0 * I - (g + b);
    }

    R = static_cast<uint8_t>(std::min(255.0, std::max(0.0, r * 255.0)));
    G = static_cast<uint8_t>(std::min(255.0, std::max(0.0, g * 255.0)));
    B = static_cast<uint8_t>(std::min(255.0, std::max(0.0, b * 255.0)));
}

void RGB2YCbCr(uint8_t R, uint8_t G, uint8_t B, double& Y, double& Cb, double& Cr)
{
    double r = R / 255.0;
    double g = G / 255.0;
    double b = B / 255.0;

    Y  = 0.299 * r + 0.587 * g + 0.114 * b;
    Cb = -0.168736 * r - 0.331264 * g + 0.5 * b;
    Cr =  0.5 * r - 0.418688 * g - 0.081312 * b;

    Cb += 0.5;
    Cr += 0.5;
}

void YCbCr2RGB(double Y, double Cb, double Cr, uint8_t& R, uint8_t& G, uint8_t& B)
{
    Cb -= 0.5;
    Cr -= 0.5;

    double r = Y + 1.402 * Cr;
    double g = Y - 0.344136 * Cb - 0.714136 * Cr;
    double b = Y + 1.772 * Cb;

    r = std::clamp(r, 0.0, 1.0);
    g = std::clamp(g, 0.0, 1.0);
    b = std::clamp(b, 0.0, 1.0);

    R = static_cast<uint8_t>(r * 255);
    G = static_cast<uint8_t>(g * 255);
    B = static_cast<uint8_t>(b * 255);
}

