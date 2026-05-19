#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

layout(std140) uniform CameraBlock {
    mat4 uView;
    mat4 uProj;
    vec3 uCameraPos;
    float uTime;
};

uniform mat4 projection;
uniform mat4 view;

out vec4 outColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    outColor = vec4(0.8f, 0.5f, 0.f, 1.f);
}