cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gpuBuffers VERSION 1.0 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build example" OFF)
option(GLM_TRAITS "glm traits if you want" ON)

set(GPUBUFFER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_array.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/indexed_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/uniform/uniform_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/traits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/userTraits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_weak_ref.hpp
)

add_library(gpuBuffers INTERFACE ${GPUBUFFER_HEADERS})

target_include_directories(gpuBuffers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits
    ${GLEW_INCLUDE_DIRS}
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
    target_include_directories(gpuBuffers INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shaders
        ${CMAKE_CURRENT_SOURCE_DIR}/include/window
    )

    # Для создания окна нужен глвф
    find_package(glfw3 REQUIRED)
    add_subdirectory(libs/shader_lib)
    add_subdirectory(libs/inspector)
    
    add_executable(gpuBuffers_example example.cpp)
    target_link_libraries(gpuBuffers_example
        gpuBuffers
        glfw
        shaderLib
        shaderIntrospector
    )

    # Эта тема нужна для копирования ассетов в папку с исполняемым файлом
    add_custom_command(TARGET gpuBuffers_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:gpuBuffers_example>/assets
    )
endif()