#version 330 core
precision mediump float;

// 3.0
in vec2 TextureCoordsVarying;

uniform sampler2D tex;

out vec4 FragColor;

float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

void main() {
    vec4 base_color = texture(tex, TextureCoordsVarying);
//     FragColor = vec4(TextureCoordsVarying.x, TextureCoordsVarying.x, TextureCoordsVarying.x, 1.0);
//     FragColor = base_color;
    float grayc = (base_color.r + base_color.g + base_color.b) / 3.0;
    FragColor = vec4(vec3(grayc), 1.0);
//     FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}