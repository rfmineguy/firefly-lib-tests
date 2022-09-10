#version 330 core

out vec4 FragColor;
in vec3 oColor;
in vec2 oTexCoord;

uniform sampler2D uTex;

void main()
{
    vec4 col = texture(uTex, oTexCoord);
    FragColor = texture(uTex, oTexCoord);
}
