#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Vertex 
{
    glm::vec3 position;
    glm::vec4 color;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};