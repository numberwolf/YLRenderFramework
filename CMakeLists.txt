#CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
#CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(opengl_studio)

MESSAGE(STATUS "FFmpeg-QuQi-Analyzer")
MESSAGE(STATUS "porschegt23@foxmail.com")
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Android")
    MESSAGE(STATUS "current platform: Android")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "current platform: Darwin")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

SET(CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_BUILD_TYPE "Debug")

SET(GLEW_H /usr/local/Cellar/glew/2.2.0/include/GL)
SET(GLFW_H /usr/local/Cellar/glfw/3.3.2/include/GLFW)
SET(GLTOOLS_H /usr/local/include)
SET(GLM_H ${CMAKE_SOURCE_DIR}/glm)

SET(GLEW_LINK /usr/local/Cellar/glew/2.2.0/lib/libGLEW.2.2.dylib)
SET(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.dylib)
SET(GLTOOLS_LINK /usr/local/lib/libgltools.dylib) # .a
SET(GLM_LINK ${CMAKE_SOURCE_DIR}/glm/build)

INCLUDE_DIRECTORIES(./)
#INCLUDE_DIRECTORIES(../)
INCLUDE_DIRECTORIES(/usr/local/include)
INCLUDE_DIRECTORIES(${GLEW_H})
INCLUDE_DIRECTORIES(${GLFW_H})
INCLUDE_DIRECTORIES(${GLTOOLS_H})
INCLUDE_DIRECTORIES(${GLM_H})

LINK_LIBRARIES(${OPENGL})
LINK_LIBRARIES(${GLEW_LINK})
LINK_LIBRARIES(${GLFW_LINK})
LINK_LIBRARIES(${GLTOOLS_LINK})
LINK_LIBRARIES(${GLM_LINK})


FIND_PACKAGE(Freetype REQUIRED)

ADD_EXECUTABLE(
        opengl_studio
        main1.cpp
#        wasm.cpp
#        main-font.cpp
#        main-sd.cpp
#        main-fbo.cpp
        glcyl/glcyl.cpp
        glcyl/common.cpp
        glcyl/buffer_object/base_buffer_object.cpp
        glcyl/buffer_object/ubo.cpp
        glcyl/texture_dimension.cpp
        glcyl/shader_program.cpp
        glcyl/frame_buffer_object.cpp
        glcyl/ft/draw_text.cpp
        glcyl/ft/font/EyerBuffer.cpp
        glcyl/ft/font/EyerFrontBox.cpp
        glcyl/ft/font/EyerFrontChar.cpp
        glcyl/ft/font/EyerFrontEngine.cpp
        glad.c
)

TARGET_LINK_LIBRARIES(opengl_studio ${FREETYPE_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(opengl_studio PRIVATE ${FREETYPE_INCLUDE_DIRS})

IF (APPLE)
    TARGET_LINK_LIBRARIES(opengl_studio "-framework OpenGL")
    TARGET_LINK_LIBRARIES(opengl_studio "-framework GLUT")
ENDIF()

# copy_directory
ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/test_1280x720_rgba.rgb ${CMAKE_SOURCE_DIR}/build/test_1280x720_rgba.rgb)

ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/glcyl/ft/front.ttf ${CMAKE_SOURCE_DIR}/build/front.ttf)

execute_process( COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/glsl ${CMAKE_SOURCE_DIR}/build/glsl)

#ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_SOURCE_DIR}/glsl/shader.vs ${CMAKE_SOURCE_DIR}/build/shader.vs)
#
#ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_SOURCE_DIR}/glsl/shader.fs ${CMAKE_SOURCE_DIR}/build/shader.fs)
#
#ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_SOURCE_DIR}/glsl/shader-gray.vs ${CMAKE_SOURCE_DIR}/build/shader-gray.vs)
#
#ADD_CUSTOM_COMMAND(TARGET opengl_studio PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_SOURCE_DIR}/glsl/shader-gray.fs ${CMAKE_SOURCE_DIR}/build/shader-gray.fs)


