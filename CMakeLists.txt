cmake_minimum_required(VERSION 3.20)

# Задаем версию стандарта
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# НАЗВАНИЕ ПРОЕКТА
project(gpuBuffers VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependency.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildLib.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildExample.cmake)

# может вы хотите какие то файлы в папках автоматически находить
file(GLOB_RECURSE ${PROJECT_NAME}_FILES
    "include/*.h"
    "include/*.inl"
)

message(STATUS "Found files: ${${PROJECT_NAME}_FILES}")

# 1. Путь к файлу-точке входа для приложения-примера (например: main.cpp)
set(${PROJECT_NAME}_EXAMPLE_FILEPATH tests/test_vertex_array.cpp)

# 2. Ручное указание файлов твоей библиотеки (если хочешь перечислить конкретные файлы)
set(${PROJECT_NAME}_LIB_H 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/indexed_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/uniform/uniform_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/vertex/vertex_weak_ref.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/traits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits/userTraits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/concepts/concepts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/BufferObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/IndexedBufferObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/shaderStorage/shader_storage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer/indirect/indirect_buffer.h
)
set(${PROJECT_NAME}_LIB_CPP "")

# 3. Подключение инклудов
set(${PROJECT_NAME}_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/gpuBuffer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/window
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpu/traits
    ${CMAKE_CURRENT_SOURCE_DIR}/include/concepts
)

# 3. Внешние либы для линковки к твоей основной библиотеке (например: gpuBuffers)
find_package(glm)
find_package(GLEW REQUIRED)
set(${PROJECT_NAME}_LIBS
    GLEW::GLEW
)

# 4. Либы, нужные ТОЛЬКО для запуска приложения-примера (например: shaderLib)
find_package(glm)
find_package(glfw3 REQUIRED)
ADD_EXTERNAL_DEPENDENCY(shaderLib https://github.com/xXxslavchickxXx/shader_lib)
ADD_EXTERNAL_DEPENDENCY(shaderIntrospector https://github.com/xXxslavchickxXx/ShaderIntrospector)
set(${PROJECT_NAME}_EXAMPLE_LIBS
    glm::glm
    glfw
    shaderLib
    shaderIntrospector
)

# Опции сборки
option(${PROJECT_NAME}_BUILD_EXAMPLE "Построить пример для проекта" OFF)
option(${PROJECT_NAME}_ASSET_TO_BUILD "Копировать ассеты в папку сборки" OFF)

# Собираем либу
create_library(${PROJECT_NAME}
    HEADERS ${${PROJECT_NAME}_LIB_H}
    INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE}
    LIBS ${${PROJECT_NAME}_LIBS}
)

if(${PROJECT_NAME}_BUILD_EXAMPLE)
    create_executable(${PROJECT_NAME}_example
        SOURCE ${${PROJECT_NAME}_EXAMPLE_FILEPATH}
        COPY_ASSETS ${PROJECT_NAME}_ASSET_TO_BUILD
        LIBS ${PROJECT_NAME} ${${PROJECT_NAME}_EXAMPLE_LIBS}
    )
endif()