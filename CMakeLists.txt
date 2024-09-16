cmake_minimum_required(VERSION 3.30.0)
project(moss-engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall)
endif()

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} -static-libgcc -static-libstdc++ -static)
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Vulkan_LIBRARIES})

include(CTest)
enable_testing()