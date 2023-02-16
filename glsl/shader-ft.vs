//#version 440 core
//#version 410 core
#version 330 core

// 2.0
//attribute vec2 position;
//varying vec2 TextureCoordsVarying;

// 3.0
//in vec3 position;
//out vec2 TextureCoordsVarying;

// 3.0 4.0
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

// 2.0-3.0
struct MediaInfo { // 3.0
    float testVal;
};

// default layout(shared)
/*
block:0,Size:144
block member idx:0,offset:0,indexs:6 -> 16 (4N?)
block member idx:1,offset:16,indexs:7 -> 32
block member idx:2,offset:48,indexs:8 -> 16
block member idx:3,offset:64,indexs:9 -> 64
block member idx:4,offset:128,indexs:10 -> 16
 */
layout(shared) uniform MVP { // in/out/uniform
    float _testVal; // 4
    float _fd[2]; // 4
    vec3 _v3;
    mat4 _m4;
    bool _b; // 4
} mvp;

uniform sampler2D tex;

uniform vec2 canvasSize;
uniform vec2 materialRatio;
uniform vec2 mirror; // X/Y, Option:  [1.0:no] [-1.0:horiMirror]

uniform float playTime;
uniform float scale;
uniform vec2 moveXY;
uniform float angle;

uniform MediaInfo u_mediaInfo;

// 3.0
out vec3 ourColor;
out vec2 TextureCoordsVarying;

// 4.0
//layout (location=0) out vec2 TextureCoordsVarying;
//layout (location=1) out vec3 ourColor;

const float PI = 3.1415926;
float duration = 1.0;
//float progress = mod(playTime, duration) / duration; // 0~1
float progress = mod(duration, duration) / duration; // 0~1

void main(void) {
    //int a = 255;
    //int b = 255 >> 2;

    float rotate = 360.0 * angle; //  * progress
    float radians = rotate * PI / 180.0;
    float s = sin(radians);
    float c = cos(radians);

    float ratioW = 1.0 * materialRatio.x * mirror.x;
    float ratioH = 1.0 * materialRatio.y * mirror.y;

    mat4 xyTransform = mat4(
            ratioW,   0.0,     0.0, 0.0,
            0.0,      ratioH,  0.0, 0.0,
            0.0,      0.0,     1.0, 0.0,
            moveXY.x, moveXY.y,   0.0, 1.0
    );

    mat4 zRotation = mat4(
        c,      -s,     0.0,    0.0, // 1:scale-x 4:y轴翻转 逆时针
        s,      c,      0.0,    0.0, // 2:scale-y 4:x轴翻转 顺时针 也可以用scale的uniform
        0.0,    0.0,    1.0,    0.0,
        0.0,    0.0,    0.0,    1.0 // 1:mv right 2:mv up
    );

    //gl_Position = xyTransform * vec4(position * scale, 0, 1); //  * scale
    //vec2 textCoord = position * 0.5 + 0.5;
    //TextureCoordsVarying = vec2(textCoord.x, 1.0 - textCoord.y);

    // xyTransform * zRotation *
    // mvp._m4 * zRotation *
    gl_Position = xyTransform * zRotation * vec4(
        position.xy,
        position.z,
        1.0);
    vec2 textCoord = vec2(position.x, position.y) * 0.5 + 0.5;
    TextureCoordsVarying = vec2(textCoord.x, 1.0 - textCoord.y);
//    ourColor = vec3(moveXY.y, moveXY.y, moveXY.y);
    ourColor = vec3(mvp._v3[0], mvp._v3[1], mvp._v3[2]);
//    ourColor = vec3(mvp._testVal, mvp._testVal, mvp._testVal);
//    ourColor = vec3(mvp._fd[0], mvp._fd[0], mvp._fd[0]);
    //ourColor = vec3(u_mediaInfo.testVal, u_mediaInfo.testVal, u_mediaInfo.testVal);
}