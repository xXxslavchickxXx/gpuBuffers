#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
//layout(location = 2) in vec3 Normal;
//layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

out vec4 outColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    outColor = vec4(0.8f, 0.5f, 0.f, 1.f);//aColor;
}