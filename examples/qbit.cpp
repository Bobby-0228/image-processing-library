#include "bmp.h"
#include "basic.h"

int main()
{
    BMP bmp("images/inputs/normal/flower.bmp");
    BMP quantized_1 = quantize_resolution(bmp, 1);
    BMP quantized_2 = quantize_resolution(bmp, 2);
    BMP quantized_4 = quantize_resolution(bmp, 4);
    BMP quantized_6 = quantize_resolution(bmp, 6);
    quantized_1.write("images/outputs/flower_quantized_1.bmp");
    quantized_2.write("images/outputs/flower_quantized_2.bmp");
    quantized_4.write("images/outputs/flower_quantized_4.bmp");
    quantized_6.write("images/outputs/flower_quantized_6.bmp");
    return 0;
}