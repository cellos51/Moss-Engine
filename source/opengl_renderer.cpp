#include "moss_renderer.hpp"

#include "util.hpp"
#include "mesh.hpp"

#include <SDL2/SDL_opengl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

const bool debugMode = true;

// Base class functions
bool OpenGLRenderer::init(SDL_Window* window)
{
    this->window = window;

    if (!init_context()) { return false; }
    if (!create_meshes()) { return false; }
    if (!create_pipelines()) { return false; }

    return true;
}

void OpenGLRenderer::drawEntity(Entity* entity)
{
    entities.push_back(entity);
}

bool OpenGLRenderer::drawFrame()
{
    static int oldWidth, oldHeight;
    SDL_GL_GetDrawableSize(window, &width, &height);

    if (width != oldWidth || height != oldHeight)
    {
        glViewport(0, 0, width, height);
        oldWidth = width;
        oldHeight = height;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_geometry();

    SDL_GL_SwapWindow(window);

    entities.clear();

    return true;
}

void OpenGLRenderer::cleanup()
{
    SDL_GLContext context = SDL_GL_GetCurrentContext();
    if (!context)
    {
        std::cerr << "Failed to get current OpenGL context: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_DeleteContext(context);
}

// Initialization functions

bool OpenGLRenderer::init_context()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, debugMode ? SDL_GL_CONTEXT_DEBUG_FLAG : 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (!SDL_GL_CreateContext(window))
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(0);
    
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool OpenGLRenderer::create_pipelines()
{
    if (!default_pipeline.create()) { return false; }

    std::vector<char> vertex_shader_code = util::readFile("assets/shaders/opengl/default_vert.glsl");
    vertex_shader_code.push_back('\0');
    std::vector<char> fragment_shader_code = util::readFile("assets/shaders/opengl/default_frag.glsl");
    fragment_shader_code.push_back('\0');
    
    if (!default_pipeline.setShaders(vertex_shader_code.data(), fragment_shader_code.data())) { return false; }

    return true;
}

bool OpenGLRenderer::create_meshes()
{
    meshes.push_back(MeshObject(mesh::loadGltf("assets/models/test.glb")));
    meshes.push_back(MeshObject(mesh::loadGltf("assets/models/icosphere.glb")));

    return true;
}

void OpenGLRenderer::draw_geometry()
{
    default_pipeline.use();

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 100.0f);

    glUniformMatrix4fv(default_pipeline.getUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(default_pipeline.getUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);

    for (Entity* entity : entities)
    {
        glm::mat4 model = entity->transform.getMatrix();
        glUniformMatrix4fv(default_pipeline.getUniformLocation("model"), 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(meshes[0].VAO);
        glDrawElements(GL_TRIANGLES, meshes[0].index_count, GL_UNSIGNED_INT, 0);
    }
}