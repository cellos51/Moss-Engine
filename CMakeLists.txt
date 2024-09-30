cmake_minimum_required(VERSION 3.30.0)
project(moss-engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-mwindows -O3 -DNDEBUG")

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Vulkan Memory Allocator
add_subdirectory(thirdparty/VulkanMemoryAllocator)

# vk-bootstrap
add_subdirectory(thirdparty/vk-bootstrap)

# fastgltf
add_subdirectory(thirdparty/fastgltf)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE thirdparty/stb)

# imgui
add_library(imgui STATIC)

target_include_directories(imgui PUBLIC thirdparty/imgui)
target_include_directories(imgui PUBLIC thirdparty/imgui/backends)

target_sources(imgui PRIVATE 
    thirdparty/imgui/imgui.h
    thirdparty/imgui/imgui.cpp

    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/backends/imgui_impl_vulkan.cpp
    thirdparty/imgui/backends/imgui_impl_sdl2.cpp
  )

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -static)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE vk-bootstrap::vk-bootstrap)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fastgltf)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stb)

include(CTest)
enable_testing()