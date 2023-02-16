//
// Created by 小老虎 on 2022/11/23.
//

#include "texture_dimension.hpp"
#include "common.hpp"

using namespace glcyl;

TextureDimension::TextureDimension() {
    m_texture_idx = 0;
    glGenTextures(1, &m_texture_id);
    // glActiveTexture(GL_TEXTURE0 + 0);
    // glBindTexture(GL_TEXTURE_2D, m_texture_id);

    std::cout << "gen texture id:" << m_texture_id << std::endl;
}

TextureDimension::TextureDimension(GLuint texture_idx) {
    m_texture_idx = texture_idx;
    glGenTextures(1, &m_texture_id);
    // glActiveTexture(GL_TEXTURE0 + texture_idx);
    // glBindTexture(GL_TEXTURE_2D, m_texture_id);

    std::cout << "gen texture id:" << m_texture_id << std::endl;
}

TextureDimension::~TextureDimension() {
    std::cout << "release texture:" << m_texture_id << std::endl;
    if (m_texture_id != 0) {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
    m_texture_idx = 0;
}

void TextureDimension::set_texture_param() {
#ifdef __EMSCRIPTEN__
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
#endif
}

bool TextureDimension::active_bind_texture() {
    if (m_texture_id <= 0) {
        return false;
    }
    glActiveTexture(GL_TEXTURE0 + m_texture_idx);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    std::cout << "active texture id:" << m_texture_id << std::endl;
    return true;
}

bool TextureDimension::active_bind_texture(GLuint texture_idx, GLuint texture_id) {
    glActiveTexture(GL_TEXTURE0 + texture_idx);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    std::cout << "static active texture id:" << texture_idx << std::endl;
    return true;
}

bool TextureDimension::set_texture_2d_gray(int width, int height, const char *input) {
    if (m_texture_id <= 0) {
        return false;
    }
    set_texture_param();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, input);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    active_bind_texture();
    return true;
}

bool TextureDimension::set_texture_2d_rgba(int width, int height, unsigned char *input_rgba) {
    if (m_texture_id <= 0) {
        return false;
    }
    set_texture_param();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, input_rgba);
#ifndef __EMSCRIPTEN__
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
    active_bind_texture();
    return true;
}

bool TextureDimension::set_texture_2d_rgba(int width, int height, const char *input_rgba) {
    if (m_texture_id <= 0) {
        return false;
    }
    set_texture_param();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, input_rgba);
#ifndef __EMSCRIPTEN__
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
    active_bind_texture();
    return true;
}

bool TextureDimension::set_texture_2d_rgba_file(int width, int height, const char *file_path) {
    int file_length = 0;
    char *input_rgba = Common::read_binary_file(file_path, &file_length);
    if (input_rgba == nullptr) {
        return false;
    }
    bool ret = set_texture_2d_rgba(width, height, input_rgba);
    delete []input_rgba;
    input_rgba = nullptr;
    return ret;
}

void TextureDimension::set_texture_idx(GLuint texture_idx) {
    m_texture_idx = texture_idx;
}

GLuint TextureDimension::texture_id() {
    return m_texture_id;
}

GLuint TextureDimension::texture_idx() {
    return m_texture_idx;
}




