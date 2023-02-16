//
// Created by 小老虎 on 2022/12/5.
//

#include "draw_text.hpp"
#include "glcyl/common.hpp"


glcyl::ft::DrawText::DrawText(unsigned char *font_data, int data_len, int font_size) {
    if (font_data != nullptr && data_len > 0) {
        m_font_box = new Eyer::EyerFrontBox(font_data, data_len, font_size);
        m_font_size = font_size;
    }
}

glcyl::ft::DrawText::~DrawText() {
    std::cout << "DrawText::~DrawText()" << std::endl;
    if (m_font_box != nullptr) {
        delete m_font_box;
        m_font_box = nullptr;
    }
    if (m_gray8_buf != nullptr) {
        delete[] m_gray8_buf;
        m_gray8_buf = nullptr;
    }
    m_font_size = 0;
    m_width = 0;
    m_height = 0;
    m_buf_len = 0;
}

bool glcyl::ft::DrawText::draw_text(const std::string &text) {
    return DrawText::draw_text(text, 4);
}

bool glcyl::ft::DrawText::draw_text(const std::string &text, size_t align) {
    if (m_font_size > 0) {
        Eyer::EyerBuffer out_buffer;

        std::wstring w_text = glcyl::Common::to_wide_string(text);
        int ret = m_font_box->GenText(
                out_buffer, m_width, m_height, const_cast<wchar_t *>(w_text.c_str()), align);
        // int ret = m_font_box->GenText(
                // out_buffer, m_width, m_height, L"OKK");
        if (ret >= 0) {
            if (m_width > 0 && m_height > 0 && out_buffer.GetPtr() != nullptr) {
                printf("debug main -> front ttf to return text_buffer %dx%d\n", m_width, m_height);
                m_buf_len = m_width * m_height;
                if (m_gray8_buf != nullptr) {
                    delete[] m_gray8_buf;
                    m_gray8_buf = nullptr;
                }

                m_gray8_buf = new uint8_t[m_buf_len];
                // m_gray8_buf = out_buffer.GetPtr();
                // int copy_len = out_buffer.GetBuffer(m_gray8_buf);
                // std::cout << "copy_len=" << copy_len << std::endl;

                memcpy(m_gray8_buf, out_buffer.GetPtr(), m_buf_len);
                return true;
            }
        }
    }
    return false;
}

int glcyl::ft::DrawText::width() {
    return m_width;
}

int glcyl::ft::DrawText::height() {
    return m_height;
}

int glcyl::ft::DrawText::buf_len() {
    return m_buf_len;
}

uint8_t *glcyl::ft::DrawText::gray_buf() {
    return m_gray8_buf;
}
