#include "bmp.h"
#include "ops.h"

int main()
{
    /* black and pepper noise*/
    
    BMP black_and_pepper("images/inputs/noisy/paint_noisy.bmp");
    ApplyMedianFilter(black_and_pepper, 3).write("images/outputs/paint_denoised_size3.bmp");
    ApplyMedianFilter(black_and_pepper, 5).write("images/outputs/paint_denoised_size5.bmp");

    /* Gaussian noise */
    std::vector<std::vector<double>> GaussianBlurKernel_3 = {
        {  1/16.0, 2/16.0,  1/16.0 },
        {  2/16.0, 4/16.0,  2/16.0 },
        {  1/16.0, 2/16.0,  1/16.0 }
        };

    std::vector<std::vector<double>> GaussianBlurKernel_5 = {
        { 1/273.0,  4/273.0,  7/273.0,  4/273.0, 1/273.0 },
        { 4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0 },
        { 7/273.0, 26/273.0, 41/273.0, 26/273.0, 7/273.0 },
        { 4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0 },
        { 1/273.0,  4/273.0,  7/273.0,  4/273.0, 1/273.0 }
        };
    BMP gaussian_noise("images/inputs/noisy/desert_noisy.bmp");
    Conv(gaussian_noise, GaussianBlurKernel_3).write("images/outputs/desert_denoised_size3.bmp");
    Conv(gaussian_noise, GaussianBlurKernel_5).write("images/outputs/desert_denoised_size5.bmp");
    ApplyNlMeans(gaussian_noise, 2, 3, 5, 7).write("images/outputs/desert_denoised_NLMeans.bmp");

    return 0;
}