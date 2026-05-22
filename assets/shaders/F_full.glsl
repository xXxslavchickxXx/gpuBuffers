#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform float uTime;

void main() {
    // Проверяем, что uniform передаётся
    FragColor = vColor * (sin(uTime) * 0.5 + 0.5);
}