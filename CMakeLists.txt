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

# OpenGL
find_package(OpenGL REQUIRED)

# SOL2
find_package(sol2 REQUIRED)

# Lua
find_package(Lua REQUIRED)

# GLAD
add_library(glad STATIC thirdparty/glad/src/gl.c)
target_include_directories(glad PUBLIC thirdparty/glad/include)

# GLM
add_subdirectory(thirdparty/glm)

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
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
    thirdparty/imgui/backends/imgui_impl_sdl2.cpp
)

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE lua)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sol2)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenGL::GL) 
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glad)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GPUOpen::VulkanMemoryAllocator)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE vk-bootstrap::vk-bootstrap)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fastgltf)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stb)

# Compile Vulkan shaders
function(compile_shader input_file output_file shader_stage)
    add_custom_command(
        OUTPUT ${output_file}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/shaders"
        COMMAND ${CMAKE_COMMAND} -E echo "Compiling shader: ${input_file}"
        COMMAND glslc -fshader-stage=${shader_stage} ${input_file} -o ${output_file}
        DEPENDS ${input_file}
        COMMENT "Compiling shader: ${input_file}"
        VERBATIM
    )
endfunction()

# List of shaders to compile
set(SHADER_SOURCES
    ${CMAKE_SOURCE_DIR}/assets/shaders/default_vert.glsl
    ${CMAKE_SOURCE_DIR}/assets/shaders/default_frag.glsl
)

# Compile shaders with appropriate stages
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    if(${SHADER_NAME} MATCHES "vert")
        set(SHADER_STAGE "vert")
    elseif(${SHADER_NAME} MATCHES "frag")
        set(SHADER_STAGE "frag")
    else()
        message(FATAL_ERROR "Unknown shader stage for shader: ${SHADER}")
    endif()

    set(COMPILED_SHADER "${CMAKE_BINARY_DIR}/assets/shaders/${SHADER_NAME}.spv")
    compile_shader(${SHADER} ${COMPILED_SHADER} ${SHADER_STAGE})
    list(APPEND COMPILED_SHADERS ${COMPILED_SHADER})
endforeach()

# Add custom target to compile shaders
add_custom_target(compile_shaders DEPENDS ${COMPILED_SHADERS})

# Ensure shaders are compiled before building the executable
add_dependencies(${CMAKE_PROJECT_NAME} compile_shaders)

# Copy assets folder after build
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)
