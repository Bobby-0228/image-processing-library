#include "bmp.h"
#include "basic.h"

int main()
{
    BMP bmp("images/inputs/normal/flower.bmp");
    crop(bmp, 182, 285, 300, 300); //in-line modification
    bmp.write("images/outputs/flower_crop.bmp");
    return 0;
}