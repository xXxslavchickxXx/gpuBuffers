cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gpuBuffers VERSION 1.0 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build example" OFF)
option(GLM_TRAITS "glm traits if you want" ON)

add_library(gpuBuffers INTERFACE)

target_include_directories(gpuBuffers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits
)

# Зависимости
find_package(GLEW REQUIRED)
target_link_libraries(gpuBuffers INTERFACE GLEW::GLEW)

# Подключение глм для трейтов
if(GLM_TRAITS)
    find_package(glm REQUIRED)
    target_link_libraries(gpuBuffers INTERFACE glm::glm)
endif()

# Запуск примера
if(BUILD_EXAMPLE)
    add_library(example_deps STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/shader/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/shaderProgram/ShaderProgram.cpp
    )
    
    target_include_directories(example_deps PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shaders
        ${CMAKE_CURRENT_SOURCE_DIR}/include/window
    )

    # Для создания окна нужен глвф
    find_package(glfw3 REQUIRED)
    
    target_link_libraries(example_deps
        gpuBuffers
    )
    
    add_executable(example example.cpp)
    target_link_libraries(example
        example_deps
        glfw
    )

    # Эта тема нужна для копирования ассетов в папку с исполняемым файлом
    add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:example>/assets
    )
endif()