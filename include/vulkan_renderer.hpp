// This file defines the VulkanRenderer class, which implements the IRenderer interface using the Vulkan API.

#pragma once

#include "renderer.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>
#include <optional>
#include <array>


class VulkanRenderer: public IRenderer
{
public:
    void initialize(SDL_Window* window) override;
    void render() override;
    void cleanup() override;
    ~VulkanRenderer() override;
private:
    struct UniformBufferObject 
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }
    };

    struct QueueFamilyIndices 
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails 
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    const size_t MAX_FRAMES_IN_FLIGHT = 2;
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const std::vector<Vertex> vertices = {
        {{-0.5475f, -0.5500f, 0.5525f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.2475f, -0.5500f, 0.5511f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.5511f, 0.5500f, -0.2475f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.5488f, 0.5500f, 0.2525f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.5525f, 0.5500f, 0.5475f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.2511f, 0.5500f, 0.2489f}, {0.0000f, 1.0000f, 0.0000f}},
        {{0.5475f, 0.5500f, -0.5525f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.2511f, -0.5500f, -0.2489f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.2511f, -0.5500f, 0.2489f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.5511f, 0.5500f, 0.2475f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.5488f, -0.0000f, 0.2525f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.2511f, 0.5500f, -0.2489f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.5488f, 0.5500f, -0.2525f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.5511f, -0.0000f, -0.2475f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.5475f, -0.0000f, 0.5525f}, {0.0000f, 0.0000f, 1.0000f}},
        {{0.5525f, -0.0000f, 0.5475f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.2525f, -0.0000f, 0.5488f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.5525f, -0.5500f, 0.5475f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.2475f, -0.0000f, 0.5511f}, {0.0000f, 0.0000f, 1.0000f}},
        {{0.2525f, -0.5500f, 0.5488f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.5475f, -0.0000f, -0.5525f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.2475f, -0.5500f, -0.5511f}, {0.0000f, 1.0000f, 0.0000f}},
        {{0.5475f, -0.5500f, -0.5525f}, {0.0000f, 0.0000f, 1.0000f}},
        {{0.2475f, -0.0000f, -0.5511f}, {1.0000f, 1.0000f, 1.0000f}},
        {{-0.2489f, 0.5500f, 0.2511f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.2489f, -0.0000f, 0.2511f}, {0.0000f, 1.0000f, 0.0000f}},
        {{0.2489f, 0.5500f, -0.2511f}, {0.0000f, 0.0000f, 1.0000f}},
        {{0.2525f, 0.5500f, 0.5488f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.2511f, -0.0000f, 0.2489f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.2525f, 0.5500f, -0.5488f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.2525f, -0.0000f, -0.5488f}, {0.0000f, 0.0000f, 1.0000f}},
        {{0.5488f, -0.0000f, -0.2525f}, {1.0000f, 1.0000f, 1.0000f}},
        {{-0.5525f, 0.5500f, -0.5475f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.2489f, -0.0000f, -0.2511f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.2525f, -0.5500f, -0.5488f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.5475f, 0.5500f, 0.5525f}, {1.0000f, 1.0000f, 1.0000f}},
        {{-0.5488f, -0.5500f, 0.2525f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.5511f, -0.5500f, -0.2475f}, {0.0000f, 1.0000f, 0.0000f}},
        {{-0.5525f, -0.5500f, -0.5475f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.5525f, -0.0000f, -0.5475f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.5488f, -0.5500f, -0.2525f}, {1.0000f, 0.0000f, 0.0000f}},
        {{0.5511f, -0.0000f, 0.2475f}, {0.0000f, 1.0000f, 0.0000f}},
        {{0.5511f, -0.5500f, 0.2475f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.2489f, -0.5500f, 0.2511f}, {1.0000f, 1.0000f, 1.0000f}},
        {{0.2489f, -0.5500f, -0.2511f}, {1.0000f, 0.0000f, 0.0000f}},
        {{-0.2475f, 0.5500f, 0.5511f}, {0.0000f, 1.0000f, 0.0000f}},
        {{0.2475f, 0.5500f, -0.5511f}, {0.0000f, 0.0000f, 1.0000f}},
        {{-0.2511f, -0.0000f, -0.2489f}, {1.0000f, 1.0000f, 1.0000f}}
    };

    const std::vector<uint16_t> indices = {
        44, 22, 40, 44, 21, 22, 0, 43, 1, 0, 36, 43, 32, 11, 29, 2, 11, 32, 3, 45, 24, 3, 35, 45, 5, 4, 9, 27, 4, 5, 46, 12, 6, 46, 26, 12, 34, 37, 38, 34, 7, 37, 19, 42, 17, 19, 8, 42, 5, 9, 33, 33, 9, 31, 25, 11, 10, 10, 11, 2, 28, 42, 8, 41, 42, 28, 25, 3, 24, 10, 3, 25, 33, 12, 26, 31, 12, 33, 37, 47, 13, 37, 7, 47, 35, 18, 45, 14, 18, 35, 16, 17, 15, 16, 19, 17, 38, 30, 34, 39, 30, 38, 23, 6, 20, 23, 46, 6, 14, 1, 18, 14, 0, 1, 15, 27, 16, 15, 4, 27, 19, 45, 18, 19, 16, 45, 20, 21, 23, 20, 22, 21, 46, 34, 30, 46, 23, 34, 29, 39, 32, 29, 30, 39, 8, 24, 28, 8, 25, 24, 7, 26, 47, 7, 33, 26, 33, 21, 44, 23, 21, 33, 28, 27, 5, 16, 27, 28, 25, 1, 43, 18, 1, 25, 29, 47, 30, 29, 11, 47, 4, 41, 9, 15, 41, 4, 31, 22, 20, 31, 40, 22, 0, 10, 36, 10, 0, 14, 13, 32, 39, 13, 2, 32, 33, 34, 23, 7, 34, 33, 25, 19, 18, 8, 19, 25, 14, 3, 10, 14, 35, 3, 36, 2, 13, 2, 36, 10, 37, 39, 38, 37, 13, 39, 20, 12, 31, 20, 6, 12, 9, 40, 31, 40, 9, 41, 42, 15, 17, 41, 15, 42, 47, 11, 43, 43, 11, 25, 28, 5, 44, 44, 5, 33, 24, 45, 28, 46, 47, 26, 28, 45, 16, 28, 40, 41, 30, 47, 46, 47, 36, 13, 44, 40, 28, 43, 36, 47
    };

    SDL_Window* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    
    VkCommandPool commandPool;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    void cleanupSwapChain();
    bool recreateSwapChain();

    void createInstance();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void updateUniformBuffer(uint32_t currentImage);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findDepthFormat();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool hasStencilComponent(VkFormat format);
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};