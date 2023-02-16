//
// Created by 小老虎 on 2022/12/5.
//

#ifndef OPENGL_STUDIO_DRAW_TEXT_HPP
#define OPENGL_STUDIO_DRAW_TEXT_HPP

#include <iostream>
#include "glcyl/ft/font/EyerFrontBox.hpp"

/*
 * function Test_drawFont() {
    const ptr = Module.cwrap("drawFont", "number", []) ();

    let width = Module.HEAPU32[ptr / 4];
    let height = Module.HEAPU32[ptr / 4 + 1 * 1];
    console.log("Test_drawFont width:", width, " height:", height);

    let buf_len = Module.HEAPU32[ptr / 4 + 1 * 2];
    console.log("Test_drawFont buf_len:", buf_len);

    let gray8_ptr = Module.HEAPU32[ptr / 4 + 1 * 3];
    let gray8_buf = copyUint8Array(Module.HEAPU8.subarray(gray8_ptr, gray8_ptr + buf_len));
    console.log("Test_drawFont gray8_buf:", gray8_buf);

    let canvas = document.getElementById("draw-text-gray8");
    canvas.width = width;
    canvas.height = height;

    let context = canvas.getContext("2d");
    let canvas_data = context.getImageData(0, 0, width, height);

    for (let x = 0; x < width; x++) {
        for (let y = 0; y < height; y++) {
            let index = y * width + x;
            canvas_data.data[4 * index + 0] = gray8_buf[index];
            canvas_data.data[4 * index + 1] = gray8_buf[index];
            canvas_data.data[4 * index + 2] = gray8_buf[index];
            canvas_data.data[4 * index + 3] = gray8_buf[index];
        }
    }

    context.putImageData(canvas_data, 0, 0);
    return 0;
} // Test_drawFont
 */

namespace glcyl {
    namespace ft {
        class DrawText {
        public:
            explicit DrawText(unsigned char* font_data, int data_len, int font_size);
            ~DrawText();

            bool draw_text(const std::string &tex);
            bool draw_text(const std::string &text, size_t align);

            int width();
            int height();
            int buf_len();
            uint8_t *gray_buf();

        private:
            Eyer::EyerFrontBox *m_font_box = nullptr;

            int m_font_size = 0;
            int m_width = 0;
            int m_height = 0;
            int m_buf_len = 0;
            uint8_t *m_gray8_buf = nullptr;

        }; // end class
    };
}; // end namespace


#endif //OPENGL_STUDIO_DRAW_TEXT_HPP
