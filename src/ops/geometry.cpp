#include<include/bmp/bmp.h>

BMP flip_horizontally()
{
    for (int y = 0; y < header.height; y++)
    {
        for (int x = 0; x < header.width / 2; x++)
        {
            int leftPixelIndex = (y * header.width + x) * bytesPerPixel;
            int rightPixelIndex = (y * header.width + (header.width - 1 - x)) * bytesPerPixel;
            //swap every values in a pixel 
            for (int i = 0; i < bytesPerPixel; i++)
            {
                //swap the pixel 
                int temp;
                temp = imageData[leftPixelIndex + i];
                imageData[leftPixelIndex + i] = imageData[rightPixelIndex + i];
                imageData[rightPixelIndex + i] = temp;
            }
        }
    }
}