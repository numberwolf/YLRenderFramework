//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_BUFFER_OBJECT_HPP
#define OPENGL_STUDIO_BUFFER_OBJECT_HPP

#include <vector>

// #include <glad/glad.h>
// // #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <glm/gtc/type_ptr.hpp>
#ifdef __EMSCRIPTEN__
// #include <GL/glext.h>
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

// #include "ubo.hpp"

namespace glcyl {
    namespace bo {
        class BaseBufferObject {
        public:
            BaseBufferObject();

            ~BaseBufferObject();

            void setup_by_default();
#ifdef __EMSCRIPTEN__
            void setup_by_emcc(GLuint _shaderProgram);
#endif
            // GLuint create_ubo(const std::string &name, UBO_TYPE type);
        private:
            // 顶点数组对象 VAO、顶点缓冲对象 VBO、EBO
            GLuint m_VBO = 0;
            GLuint m_VAO = 0;
            GLuint m_EBO = 0;
            // std::vector<glcyl::bo::UBO> m_UBO_list; // Uniform buffer object

            void setup_by_native();
            void setup_by_emcc();
        };

    } // end namespace bo
}; // end namespace


#endif //OPENGL_STUDIO_BUFFER_OBJECT_HPP
