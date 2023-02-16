//
// Created by Chang,Yanlong on 2022/11/30.
//

#include "frame_buffer_object.hpp"

glcyl::FrameBufferObject::FrameBufferObject(int width, int height) {
    //GLuint framebuffer;
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    std::cout << "FrameBufferObject gen glBindFramebuffer:" << m_framebuffer << std::endl;

    //GLuint m_out_texture;
    m_out_texture = new TextureDimension();
    std::cout << "FrameBufferObject glBindTexture:" << m_out_texture->texture_id() << std::endl;

    // m_out_texture->set_texture_2d_rgba(width, height, nullptr);
    m_width = width;
    m_height = height;

    // param
    m_out_texture->active_bind_texture();
    m_out_texture->set_texture_param();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer); // 对framebuffer帧缓冲进行绘制
    //
    // bind frame tex1
    //
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, m_out_texture->texture_id(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glcyl::FrameBufferObject::~FrameBufferObject() {
    std::cout << "release framebuffer:" << m_framebuffer << std::endl;
    if (m_framebuffer != 0) {
        glDeleteFramebuffers(1, &m_framebuffer);
    }

    if (m_out_texture != nullptr) {
        delete m_out_texture;
        m_out_texture = nullptr;
    }
    m_framebuffer = 0;
}

void glcyl::FrameBufferObject::draw_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    // glViewport(0, 0, m_width, m_height);
    //glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
}

void glcyl::FrameBufferObject::draw_framebuffer(int w, int h) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glViewport(0, 0, w, h);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void glcyl::FrameBufferObject::draw_framebuffer(int x, int y, int w, int h) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glViewport(x, y, w, h);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void glcyl::FrameBufferObject::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    m_out_texture->active_bind_texture();
}

void glcyl::FrameBufferObject::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glcyl::FrameBufferObject::draw_screen(int ctx_width, int ctx_height) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glViewport(0, 0, ctx_width, ctx_height);
    //glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
}

glcyl::TextureDimension *glcyl::FrameBufferObject::texture_object() {
    return m_out_texture;
}



