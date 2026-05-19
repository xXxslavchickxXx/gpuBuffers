#include <iostream>
#include <vector>

#include <vertex/vertex_array.hpp>
#include <window.h>
#include <shaderProgram/ShaderProgram.h>

using namespace engine::gpu;

// Тестовая структура для описания позиции точки
struct vec3 {
    float x, y, z;
};

int main()
{
    // Создаем окно
    Window win;
    glewInit();

    // Создаем конвейер и настраиваем его
    vertex_array vao;
    auto posBuffer = vao.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);
    auto idxBuffer = vao.add_index_buffer(GL_STATIC_DRAW);

    std::vector<glm::vec3> points = {
        glm::vec3( 0.3f,  0.5f,  0.f),
        glm::vec3(-0.3f,  0.5f,  0.f),
        glm::vec3( 0.3f, -0.5f,  0.f),
        glm::vec3(-0.3f, -0.5f,  0.f)
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        1, 2, 3
    };

    posBuffer->upload(points);
    idxBuffer->upload(indices);
    // Создаем шейдерную программу
    auto program = shader::ShaderProgram::from_path("assets/shaders/V.glsl", "assets/shaders/F.glsl");

    // Настриваем то, что будет происходить в игровом цикле
    auto gameLoop = [&]() {
        program.bind();
        vao.bind();

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    };

    // Запускаем игровой цикл
    win.run(gameLoop);
}