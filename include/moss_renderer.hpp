#pragma once

#include "moss_mesh.hpp"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <SDL.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

class MossRenderer
{
public:
    virtual bool init(SDL_Window* window) = 0;
    virtual bool draw() = 0;
    virtual void cleanup() = 0;
};

class VulkanRenderer: public MossRenderer
{
public:
    bool init(SDL_Window* window) override;
    bool draw() override;
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

    bool init_device();
    bool create_swapchain();
    bool recreate_swapchain();
    bool get_queues();
    bool create_graphics_pipeline();
    bool create_command_pool();
    bool create_mesh_buffers();
    bool create_command_buffers();
    bool create_sync_objects();

    void draw_geometry(VkCommandBuffer command_buffer, VkImageView image_view);

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
    
    SDL_Window* window;
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

    VkRenderPass render_pass;
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
    std::vector<Mesh> meshes;
};

class OpenGLRenderer: public MossRenderer // Placeholder for future implementation
{
    
};