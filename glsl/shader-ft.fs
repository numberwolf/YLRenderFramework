// #version 440 core
//#version 410 core
#version 330 core
precision mediump float;

// 2.0
// in vec2 TextureCoordsVarying;

// 3.0
in vec3 ourColor;
in vec2 TextureCoordsVarying;

// 4.0
// layout (location=0) in vec2 TextureCoordsVarying;
// layout (location=1) in vec3 ourColor;

uniform sampler2D tex;
uniform vec4 cropParam; // [x1, y1, w1, h1]

out vec4 FragColor;

float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

void main() {
    //     gl_FragColor = texture2D(tex, TextureCoordsVarying);

    vec4 base_color = texture(tex, TextureCoordsVarying);
//    if (base_color.r < 0.5 && base_color.g < 0.5 && base_color.b < 0.5) {
//        base_color.r = 1.0;
//    } else {
//        base_color.b = 1.0;
//        base_color.r = 0.0;
//        base_color.g = 0.0;
//    }

    base_color.a = step(0.5, base_color.r);

    base_color.g = step(0.5, base_color.r);
    base_color.b = step(0.5, base_color.r); // 0.0;
    base_color.r = 0.0;
//    vec4 base_color = vec4(vec3(texture(tex, TextureCoordsVarying).rrr), 1.0);
//    vec4 base_color = vec4(texture(tex, vec2(TextureCoordsVarying.x / 4.0f, TextureCoordsVarying.y / 4.0f)).rrr, 1.0);

    //     FragColor = vec4(TextureCoordsVarying.x, TextureCoordsVarying.x, TextureCoordsVarying.x, 1.0);
    //     return;

    //     if (TextureCoordsVarying.x > 0.5) {
    //         FragColor = vec4(ourColor, 1.0);
    //         return;
    //     }

//    float cropX = cropParam.x;
//    float cropY = cropParam.y;
//    float cropW = cropParam.z;
//    float cropH = cropParam.w;
//
//    float x1_s1 = step(cropX, TextureCoordsVarying.x);
//    float y1_s1 = step(cropY, TextureCoordsVarying.y);
//    float x2_s1 = step(TextureCoordsVarying.x, cropX + cropW);
//    float y2_s1 = step(TextureCoordsVarying.y, cropY + cropH);
//
//    float crop_s1 = x1_s1 * y1_s1 * x2_s1 * y2_s1;
//
//    base_color.r *= crop_s1;
//    base_color.g *= crop_s1;
//    base_color.b *= crop_s1;
//    base_color.a *= crop_s1;

    FragColor = base_color;
//    FragColor = vec4(ourColor, 1.0);
}