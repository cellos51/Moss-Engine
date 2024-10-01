#include "moss_mesh.hpp"

#include <glm/glm.hpp>

#include <vector>

using namespace mesh;

Mesh mesh::create_square_mesh()
{
    Mesh mesh;

    mesh.vertices = 
    {
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }
    };

    mesh.indices = { 0, 1, 2, 2, 3, 0 };

    return mesh;
}