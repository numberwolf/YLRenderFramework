//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_TEXTURE_DIMENSION_HPP
#define OPENGL_STUDIO_TEXTURE_DIMENSION_HPP

// #include <glad/glad.h>
// // #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <glm/gtc/type_ptr.hpp>
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#else
#include <glad/glad.h>
// #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

// #ifdef __EMSCRIPTEN__
// #include <../glm/gtc/type_ptr.hpp>
// #else
#include <glm/gtc/type_ptr.hpp>
// #endif


#ifdef __APPLE__
#include <glut/glut.h> // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h> // Windows FreeGlut equivalent
#endif


namespace glcyl {
    class TextureDimension {
    public:
        TextureDimension();
        explicit TextureDimension(GLuint texture_idx);
        ~TextureDimension();

        bool active_bind_texture();
        static bool active_bind_texture(GLuint texture_idx, GLuint texture_id);
        bool set_texture_2d_gray(int width, int height, const char *input);
        bool set_texture_2d_rgba(int width, int height, unsigned char *input_rgba);
        bool set_texture_2d_rgba(int width, int height, const char *input_rgba);
        bool set_texture_2d_rgba_file(int width, int height, const char *file_path);

        void set_texture_idx(GLuint texture_idx);
        void set_texture_param();

        GLuint texture_id();
        GLuint texture_idx();
    private:
        GLuint m_texture_idx = 0;
        GLuint m_texture_id = 0;


    };
}; // end namespace


#endif //OPENGL_STUDIO_TEXTURE_DIMENSION_HPP
