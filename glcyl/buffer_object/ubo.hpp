//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_UBO_HPP
#define OPENGL_STUDIO_UBO_HPP

#include <iostream>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#else
#include <glad/glad.h>
// #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

// #ifdef __EMSCRIPTEN__
// #include <../../glm/gtc/type_ptr.hpp>
// #else
#include <glm/gtc/type_ptr.hpp>
// #endif

#ifdef __APPLE__
#include <glut/glut.h> // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h> // Windows FreeGlut equivalent
#endif

#define UBO_TYPE int
#define UBO_TYPE_FLOAT 0
#define UBO_TYPE_VEC2 2
#define UBO_TYPE_VEC3 3
#define UBO_TYPE_VEC4 4

namespace glcyl {
    namespace bo {
        /*
         * Uniform Buffer Object
         */
        class UBO {
        public:
            // explicit UBO(const std::string &name, UBO_TYPE type);
            explicit UBO(GLuint shader_handle, const std::string &name);
            explicit UBO(GLuint shader_handle, const std::string &name, GLuint link_idx);
            ~UBO();

            std::vector<GLint> get_names_offsets(const std::vector<std::string> &name_strs);

            GLuint get_offset_size_by_idx(int offset_idx);

            void set_uniform(int offset_idx, float x);
            void set_uniform(int offset_idx, float x, float y);
            void set_uniform(int offset_idx, float x, float y, float z);
            void set_uniform(int offset_idx, float r, float g, float b, float a);
            void set_uniform(int offset_idx, float *mat);

            void exit_bind();

            GLint block_size();
            std::vector<GLint> offsets();

            GLuint ubo_id();
            std::string ubo_name();

            GLuint ubo_shader();
        private:
            GLuint m_ubo = 0;
            std::string m_ubo_name;

            GLint m_block_size = 0;
            std::vector<GLint> m_offsets;

            GLuint m_bind_shader = 0;
        };
    }; // end namespace bo
}; // end namespace glcyl


#endif //OPENGL_STUDIO_UBO_HPP
