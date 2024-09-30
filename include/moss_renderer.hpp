#pragma once

#include "moss_mesh.hpp"

#include <glm/glm.hpp>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>

#include <vector>
#include <stdexcept>
#include <functional>
#include <deque>
#include <span>
#include <format>
#include <iostream>
#include <unordered_map>
#include <filesystem>

class MossRenderer
{
public:
    virtual void init(SDL_Window* window) = 0;
    virtual void draw() = 0;
    virtual void cleanup() = 0;
};

class VulkanRenderer: public MossRenderer
{
public:
    void init(SDL_Window* window) override;
    void draw() override;
    void cleanup() override;
private:
    static constexpr unsigned int FRAME_OVERLAP = 2;

    struct DescriptorLayoutBuilder 
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        void add_binding(uint32_t binding, VkDescriptorType type);
        void clear();
        VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
    };

    struct DescriptorAllocator 
    {
        struct PoolSizeRatio
        {
            VkDescriptorType type;
            float ratio;
        };
        VkDescriptorPool pool;
        void init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
        void clear_descriptors(VkDevice device);
        void destroy_pool(VkDevice device);
        VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);
    };

    struct DeletionQueue
    {
        std::deque<std::function<void()>> deletors;
        void push_function(std::function<void()>&& function);
        void flush();
    };

    struct FrameData 
    {
        VkSemaphore _swapchainSemaphore, _renderSemaphore;
        VkFence _renderFence;
        VkCommandPool _commandPool;
        VkCommandBuffer _mainCommandBuffer;
        DeletionQueue _deletionQueue;
    };

    struct AllocatedImage 
    {
        VkImage image;
        VkImageView imageView;
        VmaAllocation allocation;
        VkExtent3D imageExtent;
        VkFormat imageFormat;
    };

    struct ComputePushConstants 
    {
        glm::vec4 data1;
        glm::vec4 data2;
        glm::vec4 data3;
        glm::vec4 data4;
    };

    struct AllocatedBuffer 
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        VmaAllocationInfo info;
    };

    // holds the resources needed for a mesh
    struct GPUMeshBuffers 
    {
        AllocatedBuffer indexBuffer;
        AllocatedBuffer vertexBuffer;
        VkDeviceAddress vertexBufferAddress;
    };

    // push constants for our mesh object draws
    struct GPUDrawPushConstants 
    {
        glm::mat4 worldMatrix;
        VkDeviceAddress vertexBuffer;
    };

    struct GeoSurface 
    {
        uint32_t startIndex;
        uint32_t count;
    };

    struct MeshAsset 
    {
        std::string name;
        std::vector<GeoSurface> surfaces;
        GPUMeshBuffers meshBuffers;
    };

    //test
    ComputePushConstants pushConstants{};

    SDL_Window* _window{ nullptr };   

    VmaAllocator _allocator;
    DeletionQueue _mainDeletionQueue; 
    
    VkInstance _instance; // Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
	VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkSurfaceKHR _surface; // Vulkan window surface
    VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

    int _frameNumber{ 0 };
    FrameData _frames[FRAME_OVERLAP];
	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

    AllocatedImage _drawImage;
    VkExtent2D _drawExtent;

    DescriptorAllocator globalDescriptorAllocator;

	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

    VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;

    VkPipeline _defaultPipeline;
    VkPipelineLayout _defaultPipelineLayout;

    VkFence _immFence;
    VkCommandBuffer _immCommandBuffer;
    VkCommandPool _immCommandPool;

    GPUMeshBuffers rectangle;

    void draw_background(VkCommandBuffer cmd);
    void draw_default(VkCommandBuffer cmd);
    void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void init_sync_structures();
    void init_descriptors();
    void init_pipelines();
	void init_background_pipelines();
    void init_default_pipeline();
    void init_imgui();
    void init_default_data();

    AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
    void destroy_buffer(const AllocatedBuffer& buffer);
    GPUMeshBuffers uploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);
    void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

    VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
    VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1);
    VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);
    VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags = 0);
    VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags = 0);
    VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask);
    void transition_image(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
    VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
    VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer cmd);
    VkSubmitInfo2 submit_info(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo, VkSemaphoreSubmitInfo* waitSemaphoreInfo);
    VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo imageview_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
    void copy_image_to_image(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);
    bool load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule);
    VkRenderingInfo rendering_info(VkExtent2D renderExtent, VkRenderingAttachmentInfo* colorAttachment, VkRenderingAttachmentInfo* depthAttachment);
    VkRenderingAttachmentInfo attachment_info(VkImageView view, VkClearValue* clear ,VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    
    static inline void VK_CHECK(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
            std::cout << std::format("Vulkan Error: {}\n", string_VkResult(result));
        }
    }
};

class OpenGLRenderer: public MossRenderer // Placeholder for future implementation
{
    
};