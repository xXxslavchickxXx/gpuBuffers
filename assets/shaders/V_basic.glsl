#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;

layout(std140) uniform CameraBlock {
    mat4 uView;
    mat4 uProj;
};

void main() {
    gl_Position = uProj * uView * model * vec4(aPos, 1.0);
}