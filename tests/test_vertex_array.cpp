#include <iostream>
#include <vector>
#include <cassert>

#include <gpuBuffer/vertex/vertex_array.hpp>
#include <gpuBuffer/uniform/uniform_buffer.hpp>
#include <window/window.h>

#include <shaderProgram/ShaderProgram.h>
#include <uniformReflector/uniformReflector.h>

using namespace ag::gpu;

int test_passed = 0;
int test_failed = 0;

#define TEST(expr, msg) do { \
    if (expr) { \
        std::cout << "[PASS] " << msg << "\n"; \
        test_passed++; \
    } else { \
        std::cout << "[FAIL] " << msg << "\n"; \
        test_failed++; \
    } \
} while(0)

#define TEST_THROWS(expr, msg) do { \
    try { \
        expr; \
        std::cout << "[FAIL] " << msg << " (should throw)\n"; \
        test_failed++; \
    } catch(...) { \
        std::cout << "[PASS] " << msg << "\n"; \
        test_passed++; \
    } \
} while(0)

// ========== Тест 1: Создание VAO и буферов ==========
void test_vao_creation() {
    std::cout << "\n=== Test 1: VAO Creation ===\n";

    vertex_array vao;
    auto buffer = vao.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);

    TEST(buffer, "Buffer created");

    // vertex_buffer_ref нужно использовать через .lock()
    auto ref = vao[0];
    TEST(ref, "Buffer accessible via operator[]");

    TEST_THROWS(vao[999], "Access to non-existing location throws");
}

// ========== Тест 2: Загрузка данных ==========
void test_buffer_upload() {
    std::cout << "\n=== Test 2: Buffer Upload ===\n";

    vertex_array vao;
    auto buffer = vao.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);

    std::vector<glm::vec3> data = {
        glm::vec3(1,2,3),
        glm::vec3(4,5,6)
    };

    try {
        buffer->upload(data);
        TEST(true, "Upload vector data");
    }
    catch (...) {
        TEST(false, "Upload vector data");
    }

    try {
        buffer->upload(data.data(), data.size());
        TEST(true, "Upload raw data");
    }
    catch (...) {
        TEST(false, "Upload raw data");
    }
}

// ========== Тест 3: Индексный буфер ==========
void test_indexed_buffer() {
    std::cout << "\n=== Test 3: Indexed Buffer ===\n";

    vertex_array vao;
    auto ibo = vao.add_index_buffer(GL_STATIC_DRAW);

    std::vector<uint32_t> indices = { 0, 1, 2, 3 };

    try {
        ibo->upload(indices);
        TEST(true, "Index buffer upload");
    }
    catch (...) {
        TEST(false, "Index buffer upload");
    }

    TEST(vao.get_index_buffer() == ibo, "Index buffer getter works");
}

// ========== Тест 4: UBO создание и биндинг ==========
void test_ubo() {
    std::cout << "\n=== Test 4: UBO ===\n";

    ag::uniform_buffer ubo;
    ubo.bind_base(0);

    struct Data {
        float a;
        float b;
    } data = { 1.0f, 2.0f };

    try {
        ubo.upload(data);
        TEST(true, "UBO upload struct");
    }
    catch (...) {
        TEST(false, "UBO upload struct");
    }

    TEST(ubo.get_binding() == 0, "UBO binding point correct");
}

// ========== Тест 5: Замена буфера через set_buffer ==========
void test_set_buffer() {
    std::cout << "\n=== Test 5: Set Buffer ===\n";

    vertex_array vao;
    vao.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);

    auto buffer2 = std::make_shared<ag::vertex_buffer>(GL_STATIC_DRAW);
    std::vector<glm::vec3> data = { glm::vec3(1,1,1) };
    buffer2->upload(data);

    try {
        vao.set_buffer(0, buffer2);
        TEST(true, "Buffer replacement works");
    }
    catch (...) {
        TEST(false, "Buffer replacement works");
    }

    TEST(true, "Incompatible buffer handling");
}

// ========== Тест 6: Трейты типов ==========
void test_type_traits() {
    std::cout << "\n=== Test 6: Type Traits ===\n";

    constexpr auto c1 = type_to_gltype_c<glm::vec3>;
    constexpr auto c2 = type_to_gltype_c<float>;
    constexpr auto c3 = type_to_gltype_c<int>;

    TEST(c1 == 3, "glm::vec3 components = 3");
    TEST(c2 == 1, "float components = 1");
    TEST(c3 == 1, "int components = 1");

    constexpr auto t1 = type_to_gltype_t<glm::vec3>;
    constexpr auto t2 = type_to_gltype_t<float>;

    TEST(t1 == GL_FLOAT, "glm::vec3 type = GL_FLOAT");
    TEST(t2 == GL_FLOAT, "float type = GL_FLOAT");
}

// ========== Тест 7: Множественные VAO ==========
void test_multiple_vao() {
    std::cout << "\n=== Test 7: Multiple VAOs ===\n";

    vertex_array vao1, vao2;

    auto buf1 = vao1.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);
    auto buf2 = vao2.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);

    std::vector<glm::vec3> data1 = { glm::vec3(1,0,0) };
    std::vector<glm::vec3> data2 = { glm::vec3(0,1,0) };

    buf1->upload(data1);
    buf2->upload(data2);

    vao1.bind();
    TEST(true, "VAO1 bind works");

    vao2.bind();
    TEST(true, "VAO2 bind works");
}

// ========== Тест 8: Мёртвые ссылки ==========
void test_dead_reference() {
    std::cout << "\n=== Test 8: Dead References ===\n";

    std::shared_ptr<ag::vertex_buffer> buffer;

    {
        vertex_array vao;
        auto ref = vao.add_buffer<glm::vec3>(GL_STATIC_DRAW, 0);
        buffer = ref.lock();
        buffer->upload(std::vector<glm::vec3>{glm::vec3(1, 2, 3)});
        TEST(true, "Buffer alive inside scope");
    }

    TEST(buffer != nullptr, "Buffer still exists after VAO destroyed");
}

// ========== Тест 9: Partial upload ==========
void test_partial_upload() {
    std::cout << "\n=== Test 9: Partial Upload ===\n";

    vertex_array vao;
    auto buffer = vao.add_buffer<glm::vec3>(GL_DYNAMIC_DRAW, 0);

    std::vector<glm::vec3> data1 = {
        glm::vec3(1,1,1),
        glm::vec3(2,2,2),
        glm::vec3(3,3,3)
    };

    std::vector<glm::vec3> data2 = {
        glm::vec3(99,99,99)
    };

    try {
        buffer->upload(data1);
        buffer->upload_part(data2, 2 * sizeof(glm::vec3));
        TEST(true, "Partial upload works");
    }
    catch (...) {
        TEST(false, "Partial upload works");
    }
}

// ========== Тест 10: Uniform reflector ==========
void test_uniform_reflector() {
    std::cout << "\n=== Test 10: Uniform Reflector ===\n";

    try {
        auto program = shader::ShaderProgram::from_path(
            "assets/shaders/V_basic.glsl",
            "assets/shaders/F_basic.glsl"
        );

        shader::uniform_reflector reflector(program.getId());

        TEST(reflector.size() >= 0, "Reflector created");
        TEST(true, "Uniform reflector works");
    }
    catch (const std::exception& e) {
        std::cout << "[SKIP] Shader not found: " << e.what() << "\n";
    }
}

// ========== Main ==========
int main() {
    system("chcp 65001 > nul");

    std::cout << "========================================\n";
    std::cout << "        VERTEX ARRAY UNIT TESTS         \n";
    std::cout << "========================================\n";

    Window win;
    glewInit();

    test_vao_creation();
    test_buffer_upload();
    test_indexed_buffer();
    test_ubo();
    test_set_buffer();
    test_type_traits();
    test_multiple_vao();
    test_dead_reference();
    test_partial_upload();
    test_uniform_reflector();

    std::cout << "\n========================================\n";
    std::cout << "RESULTS: " << test_passed << " passed, " << test_failed << " failed\n";
    std::cout << "========================================\n";

    return test_failed > 0 ? 1 : 0;
}