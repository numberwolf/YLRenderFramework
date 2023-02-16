//
// Created by 小老虎 on 2022/11/23.
//

#ifndef OPENGL_STUDIO_SHADER_PROGRAM_HPP
#define OPENGL_STUDIO_SHADER_PROGRAM_HPP

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

// #include <GLTools.h>
// #include <GLShaderManager.h> // Shader Manager Class

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

#ifndef OPENGL_RENDER_GRAPH_GL_SHADER_FORAMT
#define OPENGL_RENDER_GRAPH_GL_SHADER_FORAMT
#define NUMBERWOLF_STRINGIZE(x) #x
#define NUMBERWOLF_GL_SHADER(shader) "" NUMBERWOLF_STRINGIZE(shader)
#endif //OPENGL_RENDER_GRAPH_GL_SHADER_FORAMT

#ifndef OPENGL_DEFAULT_SHADERS
static const GLchar *st_default_vertexSource = (const GLchar *) NUMBERWOLF_GL_SHADER (
attribute vec3 position;
// attribute vec2 aTexCoord;
uniform sampler2D tex;
uniform mat4 xyRotation;

varying vec2 TextureCoordsVarying;

void main(void) {
    // float materialWRatio = 1.0;
    // float materialHRatio = 1.0;
    // float horiMirror = 1.0;
    // float vertMirror = 1.0;
    // float moveX = 0.0;
    // float moveY = 0.0;
    //
    // mat4 xyRotation = mat4(
    //         //c * materialW,      -s,     0.0,    0.0, // 1:scale-x 4:y轴翻转 逆时针
    //         //s,      c * materialH,      0.0,    0.0, // 2:scale-y 4:x轴翻转 顺时针 也可以用scale的uniform
    //         // 1:scale-x 4:y轴翻转 逆时针
    //         1.0 * materialWRatio * horiMirror,      0.0,     0.0,    0.0,
    //         // 2:scale-y 4:x轴翻转 顺时针 也可以用scale的uniform
    //         0.0,      1.0 * materialHRatio * vertMirror,     0.0,    0.0,
    //         0.0,    0.0,    1.0,    0.0,
    //         // 1:mv right 2:mv up
    //         0.0 + moveX,    0.0 + moveY,    0.0,    1.0
    // );

    gl_Position = xyRotation * vec4(position.xy, 0, 1); //  * scale
    // vec2 textCoord = position;
    vec2 textCoord = position.xy * 0.5 + 0.5;
    // vec2 textCoord = aTexCoord;
    TextureCoordsVarying = vec2(textCoord.x, 1.0 - textCoord.y); // 1.0 -
}
); // st_default_vertexSource -------------------------------------------------------------------------

static const GLchar *st_default_fragmentSource = (const GLchar *) NUMBERWOLF_GL_SHADER (
// #version 330 core
precision mediump float;
// 2.0
varying vec2 TextureCoordsVarying;
uniform sampler2D tex;

float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

void main() {
    gl_FragColor = texture2D(tex, TextureCoordsVarying); // FragColor
    // gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    // FragColor = texture(tex, TextureCoordsVarying);
}
); // st_default_fragmentSource -------------------------------------------------------------------------


#endif //OPENGL_RENDER_GRAPH_GL_SHADER_FORAMT

namespace glcyl {

    class ShaderProgram {
    public:
        ShaderProgram();

        ~ShaderProgram();

        bool load_by_type(const std::string &source, GLuint shader_type);
        bool load_by_source(const std::string &vertex_source, const std::string &frag_source);
        bool load_by_file_type(const std::string &vertex_path, GLuint shader_type);
        bool load_by_2_file(const std::string &vertex_path, const std::string &frag_path);

        void setUniform(const char *name, int count, float *vec);
        void setUniform(const char *name, float *mat4);
        void setUniform(const char *name, const glm::vec2 &v);
        void setUniform(const char *name, const glm::vec3 &v);
        void setUniform(const char *name, const glm::vec4 &v);
        void setUniform(const char *name, const glm::mat4 &m);
        void setUniform(const char *name, const glm::mat3 &m);
        void setUniform(const char *name, float val);
        void setUniform(const char *name, int val);
        void setUniform(const char *name, bool val);

        bool link();
        void use_begin();
        void use_end();

        GLuint getHandle();
        bool isLinked();
        const std::string log();

    private:
        GLuint m_shader_handle = 0;
        GLuint m_vertex_shader_id = 0;
        GLuint m_fragment_shader_id = 0;

        bool m_linked = false;
        std::string m_logString;
    };
}; // end namespace


#endif //OPENGL_STUDIO_SHADER_PROGRAM_HPP
