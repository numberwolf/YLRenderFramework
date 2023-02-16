//
// Created by 小老虎 on 2023/2/16.
//
//
// Created by Chang,Yanlong on 2022/11/17.
//
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "glm/gtc/type_ptr.hpp"


#include <glad/glad.h>
// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#include <glut/glut.h> // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h> // Windows FreeGlut equivalent
#endif
#include "glcyl/common.hpp"
#include "glcyl/buffer_object/ubo.hpp"
#include "glcyl/buffer_object/base_buffer_object.hpp"
#include "glcyl/shader_program.hpp"
#include "glcyl/texture_dimension.hpp"
#include "glcyl/ft/draw_text.hpp"

const GLuint DEFAULT_WINDOW_WIDTH = 800;
const GLuint DEFAULT_WINDOW_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow*, int new_screen_width, int new_screen_height)
{
    glViewport(0, 0, new_screen_width, new_screen_height);
    // projection_matrix = glm::perspective(45.0f, (GLfloat)new_screen_width / (GLfloat)new_screen_height, 0.1f, 10.0f);
}

void cursor_pos_callback(GLFWwindow* window, double, double ypos) {

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    // {
    //     if(ypos_old != -1)
    //     {
    //         camera_position.z += CAMERA_MOVEMENT_SPEED * (ypos_old - ypos);
    //         camera_position.z = glm::clamp(camera_position.z, 0.2f, 9.90f);
    //     }
    //
    //     ypos_old = ypos;
    // }
    // else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    //     ypos_old = -1;
    // }
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(
            DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
            "github.com/numberwolf", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    // glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glcyl::bo::BaseBufferObject buffer_object = glcyl::bo::BaseBufferObject();
    buffer_object.setup_by_default();

    //
    // shader
    //
    glcyl::ShaderProgram shader_program = glcyl::ShaderProgram();
    if (!shader_program.load_by_2_file("glsl/shader.vs", "glsl/shader.fs")) {
        std::cout << "err1:" << shader_program.log().c_str() << std::endl;
        exit(-1);
    }
    if (!shader_program.link()) {
        std::cout << "err2:" << shader_program.log().c_str() << std::endl;
        exit(-1);
    }

    shader_program.setUniform("tex", 0);

    // shader end

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    //glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // end vbo

    //
    // texture
    //
    glcyl::TextureDimension texture_dimension = glcyl::TextureDimension(0);
    texture_dimension.active_bind_texture();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width = 1280;
    int height = 720;
    const std::string file_path = "test_1280x720_rgba.rgb";
    texture_dimension.set_texture_2d_rgba_file(width, height, file_path.c_str());

    // aspect ratio
    float aspectW = -1.0; // init
    float aspectH = -1.0; // init
    float angle = 0.0; // 0.25 * 360° = 90°
    float moveX = 0.0;
    float moveY = 0.0;
    float OPENG_RENDER_DEFAULT_CROP_AREA[4] = {0.0, 0.0, 1.0, 1.0};

    glcyl::Common::adaptive_window(
            width, height,
            DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
            angle, // angle0-1f
            &aspectW, &aspectH);

    std::cout << "aspectW:" << aspectW << ",aspectH:" << aspectH << std::endl;

    shader_program.use_begin();
    texture_dimension.active_bind_texture();

    shader_program.setUniform("cropParam", glm::vec4(
            OPENG_RENDER_DEFAULT_CROP_AREA[0],
            OPENG_RENDER_DEFAULT_CROP_AREA[1],
            OPENG_RENDER_DEFAULT_CROP_AREA[2],
            OPENG_RENDER_DEFAULT_CROP_AREA[3]));

    shader_program.setUniform("canvasSize",
                              glm::vec2((float) DEFAULT_WINDOW_WIDTH * 1.0f, (float) DEFAULT_WINDOW_HEIGHT * 1.0f));
    shader_program.setUniform("materialRatio", glm::vec2(aspectW, aspectH));
    shader_program.setUniform("mirror", glm::vec2(1.0f, 1.0f));

    shader_program.setUniform("playTime", 0.0f);
    shader_program.setUniform("scale", 1.0f);
    shader_program.setUniform("moveXY", glm::vec2(moveX, moveY));
    shader_program.setUniform("angle", angle);

    shader_program.setUniform("u_mediaInfo.testVal", 0.1f);

    // create ubo
    glcyl::bo::UBO ubo1_object = glcyl::bo::UBO(shader_program.getHandle(), "MVP");
    std::vector<std::string> names = {"MVP._testVal", "MVP._fd", "MVP._v3", "MVP._m4", "MVP._b"};
    std::vector<GLint> offsets = ubo1_object.get_names_offsets(names);

    /*
     * layout(shared) uniform MVP { // in/out/uniform
        float _testVal; // 4
        float _fd[2]; // 4
        vec3 _v3;
        mat4 _m4;
        bool _b; // 4
    } mvp;
     */
    ubo1_object.set_uniform(0, 1.0f);
    float fd[2] = {1, 0};
    ubo1_object.set_uniform(1, fd);
    float v3[3] = {0.5, 1.0, 0.5};
    ubo1_object.set_uniform(2, v3);
    // float m4[4 * 4] = {
    //     1, 2, 3, 4,
    //     5, 6, 7, 8,
    //     1, 0, 0, 0,
    //     1, 0, 0, 0
    // };
    float m4[4 * 4] = {
            aspectW,  0.0,      0.0, 0.0,
            0.0,      aspectH,  0.0, 0.0,
            0.0,      0.0,      1.0, 0.0,
            0.0,      0.0,      0.0, 1.0
    };
    ubo1_object.set_uniform(3, m4);
    ubo1_object.set_uniform(4, false);

    processInput(window);
    glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    /******************
     * font begin
     */

    glcyl::ShaderProgram shader_program_font = glcyl::ShaderProgram();
    if (!shader_program_font.load_by_2_file("glsl/shader-ft.vs", "glsl/shader-ft.fs")) {
        std::cout << "err1:" << shader_program_font.log().c_str() << std::endl;
        exit(-1);
    }
    if (!shader_program_font.link()) {
        std::cout << "err2:" << shader_program_font.log().c_str() << std::endl;
        exit(-1);
    }

    shader_program_font.setUniform("tex", 0);

    // font begin
    std::cout << "texture_dimension to read font file" << std::endl;
    // front.ttf
    int file_length = 0;
    char *input_font = glcyl::Common::read_binary_file("front.ttf", &file_length);

    std::cout << "texture_dimension to new object" << std::endl;
    glcyl::ft::DrawText draw_text_obj(
            reinterpret_cast<unsigned char *>(input_font), file_length, 100);

    std::cout << "texture_dimension to draw" << std::endl;
    bool draw_ret = draw_text_obj.draw_text("RTT FreeType 字体渲染引擎");

    std::cout << "texture_dimension draw ret:" << draw_ret << std::endl;
    if (draw_ret) {
        std::cout <<
                  "texture_dimension to set texture 2d rgba len:"
                  << draw_text_obj.buf_len()
                  << ",w:" << draw_text_obj.width()
                  << ",h:" << draw_text_obj.height()
                  << std::endl;
        if (texture_dimension.set_texture_2d_gray(
                draw_text_obj.width(), draw_text_obj.height(),
                reinterpret_cast<const char *>(draw_text_obj.gray_buf()))) {
            std::cout << "texture_dimension draw to use shader" << std::endl;
            shader_program.use_begin();
            texture_dimension.active_bind_texture();
        }
    }

    delete[] input_font;
    input_font = nullptr;

    int width_font = draw_text_obj.width();
    int height_font = draw_text_obj.height();

    // aspect ratio
    float aspectW_font = -1.0; // init
    float aspectH_font = -1.0; // init
    float angle_font = 0.0; // 0.25 * 360° = 90°
    float moveX_font = 0.0;
    float moveY_font = 0.0;
    float OPENG_RENDER_DEFAULT_CROP_AREA_font[4] = {0.0, 0.0, 1.0, 1.0};

    glcyl::Common::adaptive_window(
            width_font, height_font,
            DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
            angle_font, // angle0-1f
            &aspectW_font, &aspectH_font);

    std::cout << "aspectW_font:" << aspectW_font << ",aspectH_font:" << aspectH_font << std::endl;

    shader_program_font.use_begin();
    texture_dimension.active_bind_texture();

    shader_program_font.setUniform("cropParam", glm::vec4(
            OPENG_RENDER_DEFAULT_CROP_AREA_font[0],
            OPENG_RENDER_DEFAULT_CROP_AREA_font[1],
            OPENG_RENDER_DEFAULT_CROP_AREA_font[2],
            OPENG_RENDER_DEFAULT_CROP_AREA_font[3]));

    shader_program_font.setUniform("canvasSize",
                                   glm::vec2((float) DEFAULT_WINDOW_WIDTH * 1.0f, (float) DEFAULT_WINDOW_HEIGHT * 1.0f));
    shader_program_font.setUniform("materialRatio", glm::vec2(aspectW_font, aspectH_font));
    shader_program_font.setUniform("mirror", glm::vec2(1.0f, 1.0f));

    shader_program_font.setUniform("playTime", 0.0f);
    shader_program_font.setUniform("scale", 1.0f);
    shader_program_font.setUniform("moveXY", glm::vec2(moveX_font, moveY_font));
    shader_program_font.setUniform("angle", angle_font);

    shader_program_font.setUniform("u_mediaInfo.testVal", 0.1f);

    // create ubo
    glcyl::bo::UBO ubo1_object_font = glcyl::bo::UBO(shader_program_font.getHandle(), "MVP");
    std::vector<std::string> names_font = {"MVP._testVal", "MVP._fd", "MVP._v3", "MVP._m4", "MVP._b"};
    std::vector<GLint> offsets_font = ubo1_object_font.get_names_offsets(names_font);

    /*
     * layout(shared) uniform MVP { // in/out/uniform
        float _testVal; // 4
        float _fd[2]; // 4
        vec3 _v3;
        mat4 _m4;
        bool _b; // 4
    } mvp;
     */
    ubo1_object.set_uniform(0, 1.0f);
    float fd_font[2] = {1, 0};
    ubo1_object.set_uniform(1, fd_font);
    float v3_font[3] = {0.5, 1.0, 0.5};
    ubo1_object.set_uniform(2, v3_font);

    float m4_font[4 * 4] = {
            aspectW,  0.0,      0.0, 0.0,
            0.0,      aspectH,  0.0, 0.0,
            0.0,      0.0,      1.0, 0.0,
            0.0,      0.0,      0.0, 1.0
    };
    ubo1_object.set_uniform(3, m4_font);
    ubo1_object.set_uniform(4, false);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    /******************
     * font end
     */

    // glViewport(0, 0, width, height);
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    float playTime = 0.0;
    // glUseProgram(shader_handle);
    while (!glfwWindowShouldClose(window))
    {
        // std::cout << "!glfwWindowShouldClose" << std::endl;
        // GLfloat now = (GLfloat) glfwGetTime();

        /*
        processInput(window);
        glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.setUniform("cropParam", glm::vec4(
                OPENG_RENDER_DEFAULT_CROP_AREA[0],
                OPENG_RENDER_DEFAULT_CROP_AREA[1],
                OPENG_RENDER_DEFAULT_CROP_AREA[2],
                OPENG_RENDER_DEFAULT_CROP_AREA[3]));
        shader_program.setUniform("canvasSize",
                                  glm::vec2((float) DEFAULT_WINDOW_WIDTH * 1.0f, (float) DEFAULT_WINDOW_HEIGHT * 1.0f));
        shader_program.setUniform("materialRatio", glm::vec2(aspectW, aspectH));
        shader_program.setUniform("mirror", glm::vec2(1.0f, 1.0f));

        shader_program.setUniform("playTime", 0.0f);
        shader_program.setUniform("scale", 1.0f);
        shader_program.setUniform("moveXY", glm::vec2(moveX, moveY));
        shader_program.setUniform("angle", angle);

        shader_program.setUniform("u_mediaInfo.testVal", 0.1f);

        // glBindVertexArray(triangleVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
         */

        /*
         * Rect
         * 0             1
         * 3             2
         */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        // glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();

        playTime += 0.01;
        // https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
        usleep(10 * 1000); // 1 millisecond = 1000 microseconds
    }

    glDisableVertexAttribArray(0);
    return 0;
} // end main
