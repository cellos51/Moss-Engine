#include "vulkanwindow.hpp"

#include <SDL2/SDL_Vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport() 
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
    bool layerFound = false;

	    for (const auto& layerProperties : availableLayers) 
	    {
	        if (strcmp(layerName, layerProperties.layerName) == 0) {
	            layerFound = true;
	            break;
	        }
	    }

	    if (!layerFound) 
	    {
	        return false;
	    }
	}

	return true;
}

void VulkanWindow::create(const char* p_title, int p_w, int p_h)
{
	if (enableValidationLayers && !checkValidationLayerSupport()) 
	{
        throw std::runtime_error("validation layers requested, but not available!");
    }

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

	VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Shooting The Moon";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Moss Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t ExtensionCount = 0;
	const char* Extensions = nullptr;

	SDL_Vulkan_GetInstanceExtensions(window, &ExtensionCount, &Extensions);
	
	createInfo.enabledExtensionCount = ExtensionCount;
	createInfo.ppEnabledExtensionNames = &Extensions;

	if (enableValidationLayers) 
	{
   		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    	createInfo.ppEnabledLayerNames = validationLayers.data();
	} 
	else 
	{
	    createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
	{
    	throw std::runtime_error("Failed to create instance. Error: " + result);
	}
}

void VulkanWindow::quit() // used before exiting the program
{
	vkDestroyInstance(instance, nullptr);

	SDL_DestroyWindow(window);
	SDL_Quit();
}