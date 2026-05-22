#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

layout(std140) uniform CameraBlock {
    mat4 uView;
    mat4 uProj;
};

uniform mat4 model;

void main() {
    vColor = aColor;
    vTexCoord = aTexCoord;
    gl_Position = uProj * uView * model * vec4(aPos, 1.0);
}