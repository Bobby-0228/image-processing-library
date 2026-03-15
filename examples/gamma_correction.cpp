#include "bmp.h"
#include "ops.h"

int main()
{
    BMP bmp("images/inputs/degraded/dark.bmp");
    BMP corrected = CorrectGamma(bmp, 2.5);
    corrected.write("images/outputs/dark_gamma_correction.bmp");
    return 0;
}