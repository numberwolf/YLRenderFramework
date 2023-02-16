//
// Created by 小老虎 on 2022/11/23.
//

#include <fstream>
#include <math.h>
#include <codecvt>
#include "common.hpp"

using namespace glcyl;

char *Common::read_binary_file(const std::string &file_path, int *file_length) {
    std::ifstream file;
    file.open(file_path, std::ifstream::binary | std::ifstream::in | std::ifstream::ate);
    *file_length = file.tellg();

    if (*file_length <= 0) {
        return nullptr;
    }

    char *result = new char[*file_length];
    file.seekg(file.beg);
    file.read(result, *file_length);
    file.close();

    return result;
}

int Common::adaptive_window(
        int materialW, int materialH, int windowW, int windowH,
        float angle, // 0-1f
        float *resultW, float *resultH)
{
    *resultW = 1.0;
    *resultH = 1.0;
    printf("RenderNode_tools_adaptive_window start\n");

    float angleFValue = angle * ANGLE_360_VALUE;
    double pi = acos(-1);
    double radians = angleFValue * pi / 180.0;
    double sinVal = abs(sin(radians));
    //double cosVal = cos(radians);
    double secondPart = 1.0 - sinVal;

    int width   = (int) floor(materialW * secondPart + materialH * sinVal);
    int height  = (int) floor(materialW * sinVal + materialH * secondPart);

    float aspectWindow = ((float) windowW) / ((float) windowH);
    float aspectMateri = ((float) width) / ((float) height);

    //*resultW = 1.0;
    //*resultH = 1.0;

    if (aspectWindow == aspectMateri) {
        // undo
    } else if (aspectWindow > aspectMateri) {

        int newH = windowH;
        int newW = (int) ((float) newH * aspectMateri);

        //console.log(newW, newH);
        *resultW = (float) newW / (float) windowW;
    } else {
        int newW = windowW;
        int newH = (int) ((float) newW / aspectMateri);

        *resultH = (float) newH / (float) windowH;
    }

    printf("RenderNode_tools_adaptive_window with "
           "material %d %d window %d %d result resw:%f resh:%f\n",
           width, height, windowW, windowH, *resultW, *resultH);

    return 0;
}

std::wstring Common::to_wide_string(const std::string &input) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}

// RenderNode_tools_adaptive_window
