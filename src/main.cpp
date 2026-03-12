#include "bmp.h"
#include "basic.h"

int main()
{
    BMP bmp("images/inputs/normal/input2.bmp");
    // crop(bmp, 180, 290, 300, 300);
    // flip_horizontally(bmp);
    bmp = quantize_resolution(bmp, 4);
    bmp.write("output.bmp");
    return 0;
}
