#include <iostream>
#include <vector>

#include <gpuBuffer.hpp>
#include <window.h>

using namespace engine::gpu;

// Тестовая структура для описания позиции точки
struct vec3 {
    float x, y, z;
};

int main()
{
    // Создаем окно
    Window win;

    // Создаем конвейер и настраиваем его
    //VertexArray vao;
    //auto posBuffer = vao.addBuffer<glm::vec3>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, 0);
    //auto idxBuffer = vao.addIndexBuffer(GL_STATIC_DRAW);

    //std::vector<glm::vec3> points = {
    //    glm::vec3( 0.3f,  0.5f,  0.f),
    //    glm::vec3(-0.3f,  0.5f,  0.f),
    //    glm::vec3( 0.3f, -0.5f,  0.f),
    //    glm::vec3(-0.3f, -0.5f,  0.f)
    //};

    //std::vector<uint32_t> indices = {
    //    0, 1, 2,
    //    1, 2, 3
    //};

    //idxBuffer->upload(indices);
    //posBuffer->upload(points);
    //// Создаем шейдерную программу
    //shader::ShaderProgram program("assets/shaders/V.glsl", "assets/shaders/F.glsl");

    // Настриваем то, что будет происходить в игровом цикле
    auto gameLoop = [&]() {
        /*program.bind();
        vao.bind();*/

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    };

    // Запускаем игровой цикл
    win.run(gameLoop);
}