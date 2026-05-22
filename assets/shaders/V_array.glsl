#version 330 core

// Массив из 3 позиций на вершину (странно, но проверяем поддержку)
layout(location = 0) in vec3 aPos[3];

layout(std140) uniform CameraBlock {
    mat4 uView;
    mat4 uProj;
};

uniform mat4 model;

void main() {
    // Берём первую позицию из массива
    gl_Position = uProj * uView * model * vec4(aPos[0], 1.0);
}