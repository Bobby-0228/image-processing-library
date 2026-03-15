#include "bmp.h"
#include "basic.h"

int main()
{
    BMP bmp("images/inputs/normal/flower.bmp");
    BMP cropped = crop(bmp, 182, 285, 300, 300);
    cropped.write("images/outputs/flower_crop.bmp");
    return 0;
}