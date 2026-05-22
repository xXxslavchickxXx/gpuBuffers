cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gpuBuffers VERSION 1.0 LANGUAGES CXX)

# ===== Опции =====
option(gpuBuffers_BUILD_EXAMPLE "Build example" OFF)
option(gpuBuffers_BUILD_TESTS "Build unit tests" OFF)
option(GLM_TRAITS "glm traits if you want" ON)

# ===== Заголовки интерфейсной библиотеки =====
set(GPUBUFFER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_array.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/indexed_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/uniform/uniform_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_weak_ref.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/traits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/userTraits.h
)

# ===== Основная библиотека (интерфейсная) =====
add_library(gpuBuffers INTERFACE ${GPUBUFFER_HEADERS})

target_include_directories(gpuBuffers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits
    ${CMAKE_CURRENT_SOURCE_DIR}/include/window
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ===== Зависимости =====
find_package(GLEW REQUIRED)
target_link_libraries(gpuBuffers INTERFACE GLEW::GLEW)

if(GLM_TRAITS)
    find_package(glm REQUIRED)
    target_link_libraries(gpuBuffers INTERFACE glm::glm)
endif()

# ===== Пример (если включён) =====
if(gpuBuffers_BUILD_EXAMPLE)
    find_package(glfw3 REQUIRED)
    
    # Подключаем зависимости (если они есть как сабмодули)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/shader_lib/CMakeLists.txt)
        add_subdirectory(libs/shader_lib)
    endif()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/inspector/CMakeLists.txt)
        add_subdirectory(libs/inspector)
    endif()
    
    add_executable(gpuBuffers_example example.cpp)
    target_link_libraries(gpuBuffers_example PUBLIC
        gpuBuffers
        glfw
    )
    
    if(TARGET shaderLib)
        target_link_libraries(gpuBuffers_example PUBLIC shaderLib)
    endif()
    if(TARGET shaderIntrospector)
        target_link_libraries(gpuBuffers_example PUBLIC shaderIntrospector)
    endif()
    
    # Копирование ассетов
    add_custom_command(TARGET gpuBuffers_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:gpuBuffers_example>/assets
    )
endif()

# ===== Тесты (если включены) =====
if(gpuBuffers_BUILD_TESTS)
    find_package(glfw3 REQUIRED)
    
    # Создаём тестовый исполняемый файл
    add_executable(gpuBuffers_tests
        tests/test_vertex_array.cpp
    )

    # Подключаем зависимости (если они есть как сабмодули)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/shader_lib/CMakeLists.txt)
        add_subdirectory(libs/shader_lib)
    endif()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/inspector/CMakeLists.txt)
        add_subdirectory(libs/inspector)
    endif()
    target_include_directories(gpuBuffers_tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu
    )
    
    target_link_libraries(gpuBuffers_tests PUBLIC
        gpuBuffers
        glfw
    )
    
    # Подключаем шейдерные библиотеки для тестов (если нужны)
    if(TARGET shaderLib)
        target_link_libraries(gpuBuffers_tests PUBLIC shaderLib)
    endif()
    if(TARGET shaderIntrospector)
        target_link_libraries(gpuBuffers_tests PUBLIC shaderIntrospector)
    endif()
    
    # Копирование ассетов для тестов
    add_custom_command(TARGET gpuBuffers_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:gpuBuffers_tests>/assets
    )
    
    # Включаем тесты в CTest (опционально)
    enable_testing()
    add_test(NAME gpuBuffersTests COMMAND gpuBuffers_tests)
    
    message(STATUS "Tests enabled. Run with: ctest --output-on-failure")
endif()