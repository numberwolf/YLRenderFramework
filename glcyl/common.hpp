//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_COMMON_HPP
#define OPENGL_STUDIO_COMMON_HPP
#include <iostream>

#ifndef ANGLE_360_VALUE
#define ANGLE_360_VALUE 360
#define ANGLE_INIT_VALUE 0.0 // angle debug reset to zero 0
#endif

namespace glcyl {
    class Common {
    public:
        static char* read_binary_file(const std::string &file_path, int *file_length);
        static int adaptive_window(
                int materialW, int materialH, int windowW, int windowH,
                float angle, // 0-1f
                float *resultW, float *resultH);
        static std::wstring to_wide_string(const std::string& input); //string to wstring

    };
}; // end namespace


#endif //OPENGL_STUDIO_COMMON_HPP
