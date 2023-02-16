 #version 330 core
 const float p_playTime = 0.5;
 const float PI = 3.1415926;
 float duration = 1.0;
 float progress = mod(p_playTime, duration) / duration; // 0~1

uniform sampler2D tex;

layout (location = 0) in vec3 position;

out vec2 TextureCoordsVarying;

 void main(void) {
     gl_Position = vec4(
             position.xy,
             position.z,
             1.0);
     vec2 textCoord = position.xy * 0.5 + 0.5;
     TextureCoordsVarying = vec2(textCoord.x, textCoord.y);
 }