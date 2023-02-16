//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_GLCYL_HPP
#define OPENGL_STUDIO_GLCYL_HPP

// #include <glad/glad.h>
// // #include <GL/glew.h>
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#else
#include <glad/glad.h>
// #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <glut/glut.h> // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h> // Windows FreeGlut equivalent
#endif

namespace glcyl {
    class GL_CYL {
    public:
        GL_CYL();
        ~GL_CYL();
    private:
    }; // end
}; // end namespace


#endif //OPENGL_STUDIO_GLCYL_HPP
