#include "bmp.h"
#include "ops.h"

int main()
{
    /* Sharpness Enhancement */
    std::vector<std::vector<double>> sharpenKernel_1 = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
        };
    
    std::vector<std::vector<double>> sharpenKernel_2 = {
        {  -1, -1,  -1 },
        { -1,  9, -1 },
        {  -1, -1,  -1 }
        };

    BMP bmp("images/inputs/degraded/blurry.bmp");

    BMP sharpen_1 = Conv( Conv(bmp, sharpenKernel_1) , sharpenKernel_1); 
    BMP sharpen_2 = Conv( Conv(bmp, sharpenKernel_2) , sharpenKernel_2); 
    
    sharpen_1.write("images/outputs/blurry_sharpened1.bmp");
    sharpen_2.write("images/outputs/blurry_sharpened2.bmp");

    return 0;
}