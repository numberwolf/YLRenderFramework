//
// Created by 小老虎 on 2022/11/23.
//

#include "ubo.hpp"

using namespace glcyl::bo;

// UBO::UBO(const std::string &name, UBO_TYPE type) {
//     glGenBuffers(1, &m_ubo);
//     glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
//
//     if (type == UBO_TYPE_FLOAT) {
//         glBufferData(GL_UNIFORM_BUFFER, sizeof(float), 0, GL_DYNAMIC_DRAW);
//     } else if (type == UBO_TYPE_VEC2) {
//         glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), 0, GL_DYNAMIC_DRAW);
//     } else if (type == UBO_TYPE_VEC3) {
//         glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), 0, GL_DYNAMIC_DRAW);
//     } else if (type == UBO_TYPE_VEC4) {
//         glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), 0, GL_DYNAMIC_DRAW);
//     }
//     m_ubo_name = name;
// }

UBO::UBO(GLuint shader_handle, const std::string &name) {
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    m_ubo_name = name;
    m_bind_shader = shader_handle;

    GLuint UniformBlockInfo = glGetUniformBlockIndex(shader_handle, m_ubo_name.c_str());
    // get offset
    glGetActiveUniformBlockiv(shader_handle, UniformBlockInfo, GL_UNIFORM_BLOCK_DATA_SIZE, &m_block_size);
    std::cout << "block:" << UniformBlockInfo << ",Size:" << m_block_size << std::endl;

    glBufferData(GL_UNIFORM_BUFFER, m_block_size, 0, GL_DYNAMIC_DRAW);

    // bind
    GLuint link_idx = 0;
    glUniformBlockBinding(shader_handle, UniformBlockInfo, link_idx);
    glBindBufferBase(GL_UNIFORM_BUFFER, link_idx, m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


UBO::UBO(GLuint shader_handle, const std::string &name, GLuint link_idx) {
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    m_ubo_name = name;
    m_bind_shader = shader_handle;

    GLuint UniformBlockInfo = glGetUniformBlockIndex(shader_handle, m_ubo_name.c_str());
    // get offset
    glGetActiveUniformBlockiv(shader_handle, UniformBlockInfo, GL_UNIFORM_BLOCK_DATA_SIZE, &m_block_size);
    std::cout << "block:" << UniformBlockInfo << ",Size:" << m_block_size << std::endl;

    glBufferData(GL_UNIFORM_BUFFER, m_block_size, 0, GL_DYNAMIC_DRAW);

    // bind
    glUniformBlockBinding(shader_handle, UniformBlockInfo, link_idx);
    glBindBufferBase(GL_UNIFORM_BUFFER, link_idx, m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UBO::~UBO() {

}

std::vector<GLint> UBO::get_names_offsets(const std::vector<std::string> &name_strs) {
    std::cout << "get_names_offsets" << std::endl;
    if (name_strs.empty()) {
        return m_offsets;
    }

    // const int len = 5;
    // char* names[] = {"MVP._testVal", "MVP._fd", "MVP._v3", "MVP._m4", "MVP._b"};
    const int len = name_strs.size();
    char *names[len];
    for (int i = 0; i < len; ++i) {
        names[i] = const_cast<char *>(name_strs[i].c_str());
    }

    GLuint indexs[len];
    glGetUniformIndices(m_bind_shader, len, names, indexs);
    GLint offset[len];
    glGetActiveUniformsiv(m_bind_shader, len, indexs, GL_UNIFORM_OFFSET, offset);

    for (int i = 0; i < len; ++i) {
        m_offsets.push_back(offset[i]);
        std::cout << "block member idx:" << i << ",offset:" << offset[i] << ",indexs:" << indexs[i] << std::endl;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);

    /*
     * layout(shared) uniform MVP { // in/out/uniform
        float _testVal; // 4
        float _fd[2]; // 4
        vec3 _v3;
        mat4 _m4;
        bool _b; // 4
    } mvp;

    float x = 1;
    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], offset[1] - offset[0], &x);
    float fd[2] = {1, 0};
    glBufferSubData(GL_UNIFORM_BUFFER, offset[1], offset[2] - offset[1], &fd);
    float v3[3] = {0.5, 1.0, 0.5};
    glBufferSubData(GL_UNIFORM_BUFFER, offset[2], offset[3] - offset[2], &v3);
    float m4[4*4] = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            1, 0, 0, 0,
            1, 0, 0, 0
    };
    glBufferSubData(GL_UNIFORM_BUFFER, offset[3], offset[4] - offset[3], &m4);
    bool b = false;
    glBufferSubData(GL_UNIFORM_BUFFER, offset[4], blockSize - offset[4], &b);
    */
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return m_offsets;
}

GLuint UBO::get_offset_size_by_idx(int offset_idx) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLsizeiptr member_size = 0;
    if (offset_idx >= m_offsets.size() - 1) {
        member_size = m_block_size - offset;
    } else {
        member_size = m_offsets[offset_idx + 1] - offset;
    }
    return member_size;
}

/*
 * typedef void (APIENTRYP PFNGLGETBUFFERSUBDATAPROC)(
 *      GLenum target, GLintptr offset, GLsizeiptr size, void *data);
 */
void UBO::set_uniform(int offset_idx, float x) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLuint mem_size = get_offset_size_by_idx(offset_idx);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, mem_size, &x);
}

void UBO::set_uniform(int offset_idx, float x, float y) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLuint mem_size = get_offset_size_by_idx(offset_idx);
    float fd[2] = {x, y};
    glBufferSubData(GL_UNIFORM_BUFFER, offset, mem_size, &fd);
}

void UBO::set_uniform(int offset_idx, float x, float y, float z) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLuint mem_size = get_offset_size_by_idx(offset_idx);
    float fd[3] = {x, y, z};
    glBufferSubData(GL_UNIFORM_BUFFER, offset, mem_size, &fd);
}

void UBO::set_uniform(int offset_idx, float r, float g, float b, float a) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLuint mem_size = get_offset_size_by_idx(offset_idx);
    float fd[4] = {r, g, b, a};
    glBufferSubData(GL_UNIFORM_BUFFER, offset, mem_size, &fd);
}

void UBO::set_uniform(int offset_idx, float *mat) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLuint offset = m_offsets[offset_idx];
    GLuint mem_size = get_offset_size_by_idx(offset_idx);
    std::cout << "set mem offset:" << offset << ", memsize:" << mem_size << std::endl;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, mem_size, mat);
}

void UBO::exit_bind() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLint UBO::block_size() {
    return m_block_size;
}

std::vector<GLint> UBO::offsets() {
    return m_offsets;
}

GLuint UBO::ubo_id() {
    return m_ubo;
}

std::string UBO::ubo_name() {
    return m_ubo_name;
}

GLuint UBO::ubo_shader() {
    return m_bind_shader;
}








