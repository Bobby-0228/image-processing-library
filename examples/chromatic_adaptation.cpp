#include "bmp.h"
#include "ops.h"
#include <cmath>

#define rad(deg) (deg*2*M_PI/360)

int main()
{
    BMP bmp1("images/inputs/degraded/color_checker.bmp");
    BMP bmp2("images/inputs/degraded/color_checker2.bmp");
    BMP bmp3("images/inputs/degraded/sea.bmp");
    BMP bmp4("images/inputs/degraded/table.bmp");
    
    /* Chromatic Adaptation: Gray wolrd*/
    BMP bmp1_gw = CorrectColorTemperature(bmp1, 0); bmp1_gw.write("images/outputs/color_checker_gray_world.bmp");
    BMP bmp2_gw = CorrectColorTemperature(bmp2, 0); bmp2_gw.write("images/outputs/color_checker2_gray_world.bmp");
    BMP bmp3_gw = CorrectColorTemperature(bmp3, 0); bmp3_gw.write("images/outputs/sea_gray_world.bmp");
    BMP bmp4_gw = CorrectColorTemperature(bmp4, 0); bmp4_gw.write("images/outputs/table_gray_world.bmp");

    /* Chromatic Adaptation: Max RGB */
    BMP bmp1_mrgb = CorrectColorTemperature(bmp1, 1); bmp1_mrgb.write("images/outputs/color_checker_max_RGB.bmp");
    BMP bmp2_mrgb = CorrectColorTemperature(bmp2, 1); bmp2_mrgb.write("images/outputs/color_checker2_max_RGB.bmp");
    BMP bmp3_mrgb = CorrectColorTemperature(bmp3, 1); bmp3_mrgb.write("images/outputs/sea_max_RGB.bmp");
    BMP bmp4_mrgb = CorrectColorTemperature(bmp4, 1); bmp4_mrgb.write("images/outputs/table_max_RGB.bmp");

    /* Enhancement */
    BMP bmp1_enhanced = EnhanceImage(bmp1_gw, rad(15), 1.7, 1.8); bmp1_enhanced.write("images/outputs/color_checker_enhanced.bmp");
    BMP bmp2_enhanced = EnhanceImage(bmp2_mrgb, rad(5), 1.2, 1.5); bmp2_enhanced.write("images/outputs/color_checker2_enhanced.bmp");
    BMP bmp3_enhanced = EnhanceImage(bmp3_mrgb, rad(5), 1.1, 1.0); bmp3_enhanced.write("images/outputs/sea_enhanced.bmp");
    BMP bmp4_enhanced = EnhanceImage(bmp4_gw, rad(60), 1.3, 0.7); bmp4_enhanced.write("images/outputs/table_enhanced.bmp");

    /* Color Temperature Adjustment: Warm*/
    AdjustTemp(bmp1_enhanced, -0.1, 0.1).write("images/outputs/color_checker_warm.bmp");
    AdjustTemp(bmp2_enhanced, -0.1, 0.1).write("images/outputs/color_checker2_warm.bmp");
    AdjustTemp(bmp3_enhanced, -0.1, 0.1).write("images/outputs/sea_warm.bmp");
    AdjustTemp(bmp4_enhanced, -0.1, 0.1).write("images/outputs/table_warm.bmp");

    /* Color Temperature Adjustment: Cold*/
    AdjustTemp(bmp1_enhanced, 0.1, -0.1).write("images/outputs/color_checker_cold.bmp");
    AdjustTemp(bmp2_enhanced, 0.1, -0.1).write("images/outputs/color_checker2_cold.bmp");
    AdjustTemp(bmp3_enhanced, 0.1, -0.1).write("images/outputs/sea_cold.bmp");
    AdjustTemp(bmp4_enhanced, 0.1, -0.1).write("images/outputs/table_cold.bmp");

    return 0;
}