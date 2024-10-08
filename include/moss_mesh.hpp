#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <filesystem>

struct Vertex 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    glm::vec4 color;
};

typedef uint32_t Index;

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

namespace mesh
{
    Mesh loadGLTF(std::filesystem::path path);
    Mesh createSquare();
}