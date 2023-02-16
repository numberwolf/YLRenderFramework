//
// Created by 小老虎 on 2022/11/23.
//

#include <cstdio>
#include "base_buffer_object.hpp"
using namespace glcyl::bo;

/***
 * CANVAS
 * O(0,0) ----------------> X (1.0)
 * |
 * |
 * |
 * |
 * |
 * V
 * Y (1.0)
 */
/*  -----------------------> 2.0 <----------------------
 * GL
 *                            Y (1.0)
 *                            ^
 *                            |
 *                            |
 *                            |
 *                            |
 *                            |
 * X(-1.0) <---------------- O(0,0) ----------------> X (1.0)
 *                            |
 *                            |
 *                            |
 *                            |
 *                            |
 *                            V
 *                            Y (-1.0)
 */
/* After to Shader
 * -----------------------> 1.0 <----------------------
 * GL
 *
 * O(0,0) -------------------------------------> X (1.0)
 * |
 * |
 * |
 * |
 * |
 * |
 * |
 * |
 * |
 * |
 * V
 * Y (1.0)
 */
/***
 * GL After trans - Crop Desc
 * O(0,0) ----------------> X (1.0)
 * |
 * |
 * |
 * |     (0,0) ------------- X=1.0
 * |      |
 * |      |     [Material Area]
 * |      |
 * |     Y=1.0 ------------- (1.0, 1.0)
 * |
 * |
 * |
 * V
 * Y (1.0)
 */

/*  -----------------------> 2.0 <----------------------
 * GL
 *                            Y (1.0)
 *                            ^
 *                            |
 *                            |
 *                            |
 *                            |
 *                            |
 * X(-1.0) <---------------- O(0,0) ----------------> X (1.0)
 *                            |
 *                            |
 *                            |
 *                            |
 *                            |
 *                            V
 *                            Y (-1.0)
 */

#define RE_COMPUTE_POSITION(x) ((x) * 0.5 + 0.5);

#ifdef __EMSCRIPTEN__
const float emcc_position[] = {
        // 1/2 三角切割
        // -1.0f, -1.0f, 1.0f,
        // 1.0f, -1.0f, 1.0f,
        // -1.0f, 1.0f, 1.0f,
        // 1.0f, 1.0f, 1.0f

        // -1.0f, -1.0f, 1.0f,
        // -1.0f, -1.0f, 1.0f,
        // -1.0f, 1.0f, 1.0f,
        // -1.0f, 1.0f, 1.0f

        // -1.0f, 1.0f, 1.0,
        // 1.0f, -1.0f, 1.0,
        // 1.0f, 1.0f, 1.0,
        //
        // -1.0f, -1.0f, 1.0,
        // 1.0f, -1.0f, 1.0,
        // -1.0f, 1.0f, 1.0

        // first triangle
        -1.0f, 1.0f, 1.0,
        1.0f, -1.0f, 1.0,
        1.0f, 1.0f, 1.0,
        // second triangle
        -1.0f, -1.0f, 1.0,
        1.0f, -1.0f, 1.0,
        -1.0f, 1.0f, 1.0,

}; // position

const float emcc_texcoords[] = {
        // first triangle
        1.0f, 1.0 - 1.0f, // 右下 // xyz
        0.0f, 1.0 - 0.0f, // 左上
        0.0f, 1.0 - -1.0f, // 左下

        // second triangle
        1.0f, 1.0 - 0.0f, // 右上
        0.0f, 1.0 - 0.0f, // 左上
        1.0f, 1.0 - 1.0f, // 右下
};

const GLuint emcc_indices[] = {
        0, 1, 2, // first triangle
        3, 4, 5, // second triangle
};
#endif

//    8. 顶点数据
const float default_vertices[] = {
        // position        // colors         // texture1
        // 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上 1
        // 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下 2
        // -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下 3
        // -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左上 4

        1.0f, 1.0f, 0.0f, // 右上 1
        1.0f, -1.0f, 0.0f, // 右下 2
        -1.0f, -1.0f, 0.0f, // 左下 3
        -1.0f, 1.0f, 0.0f, // 左上 4

        /*
         * 正方形：
         * 0             1   --> 3             2
         * 3             2   --> 4             1
         */

        // 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右下 1
        // 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右上 2
        // -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左上 3
        // -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左下 4

        // 旋转一下
        /*
         * 正方形：
         * 0             1   --> 3             2 --> 4  3 顺时针PI/2
         * 3             2   --> 4             1 --> 1  2
         */
        // -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 1
        // 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 2
        // 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
        // -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 4

        // x y z
        //  1.0f, 1.0f            -1.0f, 1.0f
        //
        //
        //  1.0f, -1.0f           -1.0f, -1.0f
        // -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
        // -1.0f, -1.0f , 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
        // -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        // -1.0f, 1.0f , 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左上
}; // default_vertices

const GLuint default_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
};




BaseBufferObject::BaseBufferObject() {

}

BaseBufferObject::~BaseBufferObject() {

}

void BaseBufferObject::setup_by_native() {
    /*
     * 顶点缓冲对象 VBO
     */
    printf("vbo start");
    glGenBuffers(1, &m_VBO);
    printf("vbo glBindBuffer\n");
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    printf("vbo glBufferData\n");
    glBufferData(GL_ARRAY_BUFFER, sizeof(default_vertices), default_vertices, GL_STATIC_DRAW);

    /*
     * 顶点属性指针 VAO
     */
    printf("vao start\n");
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // 设置指针
    printf("ptr start\n");
    int vertex_len_size = 3;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_len_size * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    /*
    int vertex_len_size = 8;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_len_size * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_len_size * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_len_size * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
     */

    /*
     * 顶点缓冲对象 EBO
     */
    printf("ebo start\n");
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(default_indices), default_indices, GL_STATIC_DRAW);
}

#ifdef __EMSCRIPTEN__
void BaseBufferObject::setup_by_emcc(GLuint _shaderProgram) {
    /*
     * 顶点缓冲对象 VBO
     */
    // vbo setup
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(emcc_position), emcc_position, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), emcc_position, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(default_vertices), default_vertices, GL_STATIC_DRAW);


    // ptr
    GLint m_loc = glGetAttribLocation(_shaderProgram, "position");
    glEnableVertexAttribArray(m_loc);

    int vertex_pos_size = 3;
    int vertex_group_size = 3;
    glVertexAttribPointer(m_loc, vertex_pos_size, GL_FLOAT, GL_FALSE, vertex_group_size * sizeof(float), (void *) 0);

    // GLint m_texcoord = glGetAttribLocation(_shaderProgram, "aTexCoord");
    // glEnableVertexAttribArray(m_texcoord);
    // glVertexAttribPointer(m_texcoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), emcc_texcoords);


    /*
     * 顶点缓冲对象 EBO
     */
    // printf("ebo start\n");
    // glGenBuffers(1, &m_EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(default_indices), default_indices, GL_STATIC_DRAW);
}
#endif

void BaseBufferObject::setup_by_default() {
// #ifdef __EMSCRIPTEN__
//     setup_by_emcc();
// #else
    setup_by_native();
// #endif
}

// GLuint BaseBufferObject::create_ubo(const std::string &name, UBO_TYPE type) {
//     /*
//      * Uniform Buffer Object
//      */
//     bo::UBO ubo = bo::UBO(name, type);
//     m_UBO_list.push_back(ubo);
//     return ubo.ubo_id();
// }
