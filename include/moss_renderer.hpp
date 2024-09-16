#pragma once

#include <moss_types.hpp>

#include <SDL_vulkan.h>

class MossRenderer
{
public:
    virtual void init(SDL_Window* window) = 0;
    virtual void cleanup() = 0;
};

class VulkanRenderer: public MossRenderer
{
public:
    void init(SDL_Window* window) override;
    void cleanup() override;
private:
    static constexpr unsigned int FRAME_OVERLAP = 2;

    struct FrameData 
    {
        VkCommandPool _commandPool;
        VkCommandBuffer _mainCommandBuffer;
    };

    int _frameNumber{ 0 };

    SDL_Window* _window{ nullptr };    
    
    VkInstance _instance;// Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
	VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands

    FrameData _frames[FRAME_OVERLAP];

	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkSurfaceKHR _surface;// Vulkan window surface
    VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void init_sync_structures();

    void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
};

class OpenGLRenderer: public MossRenderer // Placeholder for future implementation
{
    
};