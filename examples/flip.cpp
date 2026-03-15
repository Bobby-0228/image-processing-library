#include "bmp.h"
#include "basic.h"

int main()
{
    BMP bmp("images/inputs/normal/mountain.bmp");

    BMP flipped_horizontally = flip_horizontally(bmp);
    flipped_horizontally.write("images/outputs/mountain_flip_hor.bmp");

    BMP flipped_vertically = flip_vertically(bmp);
    flipped_vertically.write("images/outputs/mountain_flip_ver.bmp");
    return 0;
}