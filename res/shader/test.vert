#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//uniform vec3 uColor;
out vec3 oColor;
out vec2 oTexCoord;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    oColor = aColor;
    oTexCoord = aTexCoord;
}
