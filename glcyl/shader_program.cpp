//
// Created by 小老虎 on 2022/11/23.
//

#include <fstream>
#include <vector>

#include "shader_program.hpp"

using namespace glcyl;

ShaderProgram::ShaderProgram() {
    m_shader_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if (m_shader_handle != 0) {
        glDeleteProgram(m_shader_handle); // glDeleteShader
        m_shader_handle = 0;
    }

    m_vertex_shader_id = 0;
    m_fragment_shader_id = 0;

    m_linked = false;
    m_logString = "";
}

bool ShaderProgram::load_by_type(const std::string & source, GLuint shader_type) {
    /*
     * shader
     */
    GLuint shader_id = glCreateShader(shader_type);
    if (shader_type == GL_VERTEX_SHADER) {
        m_vertex_shader_id = shader_id;
    } else {
        m_fragment_shader_id = shader_id;
    }

    // Compile Shader
    const GLchar* source_ptr = source.c_str();
    glShaderSource(shader_id, 1, &source_ptr, nullptr);
    glCompileShader(shader_id);

    // Check if the compilation worked
    GLint is_compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_TRUE)
    {
        glAttachShader(m_shader_handle, shader_id);
        m_logString = "Shader compiled successfully.";
    } else {
        GLint maxLength = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0) {
            std::vector<char> infoLog(maxLength + 1);
            glGetShaderInfoLog(shader_id, maxLength, nullptr, infoLog.data());
            m_logString = std::string(std::begin(infoLog), std::end(infoLog));
        }
        return false;
    }

    return true;
}

bool ShaderProgram::load_by_source(const std::string &vertex_source, const std::string &frag_source) {
    /*
     * vertex shader
     */
    if (!load_by_type(vertex_source, GL_VERTEX_SHADER)) {
        return false;
    }

    /*
     * fragment shader
     */
    return load_by_type(frag_source, GL_FRAGMENT_SHADER);
}

bool ShaderProgram::load_by_file_type(const std::string &shader_path, GLuint shader_type) {
    if (access(shader_path.c_str(), F_OK) == 0) {
        std::string shaderCode;
        std::ifstream ShaderStream(shader_path, std::ios::in);
        if (ShaderStream.is_open()) {
            std::string Line = "";
            while (getline(ShaderStream, Line)) {
                shaderCode += "\n" + Line;
            }
            ShaderStream.close();
            if (!load_by_type(shaderCode, shader_type)) {
                return false;
            }
        }
    } else {
        m_logString = "read shader file error:" + shader_path;
        return false;
    }

    return true;
}

bool ShaderProgram::load_by_2_file(const std::string &vertex_path, const std::string &frag_path) {
    if (!load_by_file_type(vertex_path, GL_VERTEX_SHADER)) {
        return false;
    }

    return load_by_file_type(frag_path, GL_FRAGMENT_SHADER);
}

void ShaderProgram::setUniform(const char *name, int count, float *vec)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1) {
        if (count == 2) {
            glUniform2fv(location, count, vec);
        } else if (count == 3) {
            glUniform3fv(location, count, vec);
        } else if (count == 4) {
            glUniform4fv(location, count, vec);
        } else {
            // undo
        }
    }
}

void ShaderProgram::setUniform(const char *name, float *mat4) {
    GLint location = glGetUniformLocation(m_shader_handle, name);
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, mat4);
}

void ShaderProgram::setUniform(const char *name, const glm::vec2 &v) {
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform2fv(location, 1, value_ptr(v));
}

void ShaderProgram::setUniform(const char *name, const glm::vec3 &v)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform3fv(location, 1, value_ptr(v));
}

void ShaderProgram::setUniform(const char *name, const glm::vec4 &v)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform4fv(location, 1, value_ptr(v));
}

void ShaderProgram::setUniform(const char *name, const glm::mat4 & m)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::setUniform(const char *name, const glm::mat3 & m)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::setUniform(const char *name, float val)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform1f(location, val);
}

void ShaderProgram::setUniform(const char *name, int val)
{
    // glUniform1i(
    //         glGetUniformLocation(
    //                 renderGraph->m_shaderProgram,
    //                 tex_name
    //         ),
    //         GL_TEXTURE_INDEX_OF_CANVAS);
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform1i(location, val);
}

void ShaderProgram::setUniform(const char *name, bool val)
{
    GLint location = glGetUniformLocation(m_shader_handle, name);

    if (location != -1)
        glUniform1i(location, val);
}

bool ShaderProgram::link() {
    glLinkProgram(m_shader_handle);

    // Check if the linking worked
    GLint is_linked = 0;
    glGetProgramiv(m_shader_handle, GL_LINK_STATUS, &is_linked);

    if (is_linked != GL_TRUE) {
        // @TODO release && exit
        GLint maxLength = 0;
        glGetProgramiv(m_shader_handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetShaderInfoLog(m_shader_handle, maxLength, NULL, infoLog.data());
        m_logString = std::string(begin(infoLog), end(infoLog));

        //The program is useless now.
        glDeleteProgram(m_shader_handle); // glDeleteShader
        m_shader_handle = 0;
        return false;
    }

    m_linked = true;
    return true;
}

void ShaderProgram::use_begin() {
    glUseProgram(m_shader_handle);
}

void ShaderProgram::use_end() {
    glUseProgram(0);
}

GLuint ShaderProgram::getHandle() {
    return m_shader_handle;
}

bool ShaderProgram::isLinked() {
    return m_linked;
}

const std::string ShaderProgram::log() {
    return m_logString;
}


