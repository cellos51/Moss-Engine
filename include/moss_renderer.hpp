#pragma once

#include "moss_mesh_instance.hpp"
#include "moss_camera.hpp"
#include "mesh.hpp"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>
#include <memory>

class Renderer
{
public:
    virtual bool init(SDL_Window* window) = 0;
    virtual void drawEntity(MeshInstance* entity) = 0;
    virtual bool drawFrame() = 0;
    virtual void cleanup() = 0;

    Camera* camera = nullptr;
protected:
    SDL_Window* window = nullptr;
};

class VulkanRenderer: public Renderer
{
public:
    bool init(SDL_Window* window) override;
    void drawEntity(MeshInstance* entity) override;
    bool drawFrame() override;
    void cleanup() override;
private:
    struct AllocatedBuffer
    {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

    struct AllocatedImage
    {
        VkImage image;
        VkImageView image_view;
        VmaAllocation allocation;
    };

    struct MeshRegion
    {
        VkDeviceSize index_offset;
        uint32_t index_count;
    };

    struct UniformBufferObject
    {
        glm::mat4 model;
    };

    // Initialization functions
    bool init_device(); // Create Vulkan instance and select physical device
    bool get_queues(); // Retrieve queue handles for graphics and presentation
    bool create_command_pool(); // Create a command pool for command buffer allocation
    bool create_swapchain(); // Create the swap chain
    bool prepare_images(); // Transition swapchain images and depth image from undefined
    bool create_mesh_buffers(); // Create buffers for mesh data
    bool create_uniform_buffers(); // Create buffers for uniform data
    bool create_descriptor_sets(); // Create a descriptor pool for uniform buffer descriptors
    bool create_command_buffers(); // Allocate and set up command buffers for rendering
    bool create_sync_objects(); // Create semaphores and fences for synchronization
    bool create_graphics_pipeline(); // Set up the graphics pipeline (shaders, render pass, etc.)

    // Drawing functions
    void draw_geometry(VkCommandBuffer command_buffer, VkImageView image_view);

    // Helper functions
    void updateUniformBuffer(UniformBufferObject& ubo);
    void updateDynamicUniformBuffer(std::vector<UniformBufferObject>& ubos);
    bool recreateSwapchain();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void transitionImageLayout(VkCommandBuffer command_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout);
    bool beginSingleTimeCommands(VkCommandBuffer& command_buffer);
    bool endSingleTimeCommands(VkCommandBuffer& command_buffer, VkQueue queue);
    VkResult createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, AllocatedBuffer& buffer);
    void destroyBuffer(AllocatedBuffer& buffer);
    VkResult createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memory_usage, AllocatedImage& image);
    void destroyImage(AllocatedImage& image);
    VkResult createImageView(VkFormat format, VkImageAspectFlags aspect, AllocatedImage& image_view);
    void destroyImageView(AllocatedImage& image_view);
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
    
    VkSurfaceKHR surface;
    VmaAllocator allocator;
    vkb::Instance instance;
    vkb::InstanceDispatchTable inst_disp;
    vkb::Device device;
    vkb::DispatchTable disp;
    vkb::Swapchain swapchain;

    VkQueue graphics_queue;
    VkQueue present_queue;

    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;
    AllocatedImage depth_image;

    VkDescriptorSetLayout descriptor_set_layout;
    VkDescriptorPool descriptor_pool;
    std::vector<VkDescriptorSet> descriptor_sets;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffers;

    std::vector<VkSemaphore> available_semaphores;
    std::vector<VkSemaphore> finished_semaphore;
    std::vector<VkFence> in_flight_fences;
    std::vector<VkFence> image_in_flight;
    size_t current_frame = 0;

    AllocatedBuffer vertex_buffer;
    AllocatedBuffer index_buffer;
    VkDeviceSize dynamic_alignment;
    std::vector<AllocatedBuffer> uniform_buffers;
    std::unordered_map<std::string, MeshRegion> mesh_regions; // Example implementation for multiple meshes
    std::vector<MeshInstance*> entities; // Example implementation for drawing multiple entities
};

class OpenGLRenderer: public Renderer
{
public:
    bool init(SDL_Window* window) override;
    void drawEntity(MeshInstance* entity) override;
    bool drawFrame() override;
    void cleanup() override;
private:
    class Pipeline
    {
    private:
        GLuint shader_program;
        std::unordered_map<GLchar, GLint> uniforms;
    public:
        ~Pipeline()
        {
            glDeleteProgram(shader_program);
        }
        bool create()
        {
            shader_program = glCreateProgram();
            if (shader_program == 0)
            {
                std::cerr << "Failed to create OpenGL program" << std::endl;
                return false;
            }
            return true;
        }
        void use()
        {
            static GLuint bound_program = -1;
            if (shader_program != bound_program)
            {
                glUseProgram(shader_program);
                bound_program = shader_program;
            }
        }
        bool setShaders(const GLchar* vertex_shader_code, const GLchar* fragment_shader_code)
        {
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
            glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);

            glCompileShader(vertex_shader);
            glCompileShader(fragment_shader);

            GLint success;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char info_log[512];
                glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
                std::cerr << "Failed to compile vertex shader: " << info_log << std::endl;
                return false;
            }

            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char info_log[512];
                glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
                std::cerr << "Failed to compile fragment shader: " << info_log << std::endl;
                return false;
            }

            glAttachShader(shader_program, vertex_shader);
            glAttachShader(shader_program, fragment_shader);

            glLinkProgram(shader_program);

            glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char info_log[512];
                glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
                std::cerr << "Failed to link program: " << info_log << std::endl;
                return false;
            }

            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            return true;
        }
        GLint getUniformLocation(const GLchar* name)
        {
            if (uniforms.find(*name) == uniforms.end())
            {
                uniforms[*name] = glGetUniformLocation(shader_program, name);
            }
            return uniforms[*name];
        }
    };

    struct MeshObject
    {
        MeshObject(Mesh mesh)
        {
            index_count = mesh.indices.size();

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(Index), &mesh.indices[0], GL_STATIC_DRAW);

            // Vertex positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);

            // Vertex normals
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            // Vertex texture coords
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(2);

            // Vertex colors
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(3);

            // Don't unbind the VAO, we use only a single one for all entities
            glBindVertexArray(0); 
        }
        GLuint VAO, VBO, EBO;
        GLuint index_count;
    };

    bool init_context();
    bool create_meshes();
    bool create_pipelines();

    void draw_geometry();

    int width, height;
    GLuint VAO, VBO, EBO;
    Pipeline default_pipeline;
    std::vector<MeshObject> meshes;
    std::vector<MeshInstance*> entities;
};