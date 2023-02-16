//
// Created by Chang,Yanlong on 2022/11/30.
//

#ifndef OPENGL_STUDIO_FRAME_BUFFER_OBJECT_H
#define OPENGL_STUDIO_FRAME_BUFFER_OBJECT_H

#include <iostream>
#include "texture_dimension.hpp"

namespace glcyl {
    class FrameBufferObject {
    public:
        FrameBufferObject(int width, int height);
        ~FrameBufferObject();

        void begin();
        void end();
        void draw_framebuffer();
        void draw_framebuffer(int w, int h);
        void draw_framebuffer(int x, int y, int w, int h);
        static void draw_screen(int ctx_width, int ctx_height);

        TextureDimension *texture_object();
    private:
        GLuint m_framebuffer = 0;
        TextureDimension *m_out_texture = nullptr;
        int m_width = 0;
        int m_height = 0;
    }; // FrameBufferObject
}; // end namespace glcyl


#endif //OPENGL_STUDIO_FRAME_BUFFER_OBJECT_H
