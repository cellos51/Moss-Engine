#include "vulkanwindow.hpp"

#include <SDL2/SDL_Vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <optional>

#include "math.hpp"

const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) 
{
    QueueFamilyIndices indices;

   	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
	    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
	    {
	        indices.graphicsFamily = i;
	    }

	    if (indices.isComplete()) 
	    {
        	break;
    	}

    	i++;
	}

    return indices;
}

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

void VulkanWindow::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) 
	{
    	throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
	    if (isDeviceSuitable(device)) {
	        physicalDevice = device;
	        break;
	    }
	}

	if (physicalDevice == VK_NULL_HANDLE) {
    	throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool VulkanWindow::isDeviceSuitable(VkPhysicalDevice device) 
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}

void VulkanWindow::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	if (enableValidationLayers) 
	{
	    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	    createInfo.ppEnabledLayerNames = validationLayers.data();
	} 
	else 
	{
	    createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
	{
    	throw std::runtime_error("failed to create logical device! THIS IS NOT POGGERS!!! :<");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

bool VulkanWindow::checkValidationLayerSupport() 
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

	pickPhysicalDevice();
	createLogicalDevice();
}

void VulkanWindow::camera(Vector2 pos) // used before exiting the program
{
	cameraPos.lerp(cameraPos, pos, lerpAmount * Time::deltaTime());

	cameraPos.x = std::clamp(cameraPos.x, pos.x - clampAmount, pos.x + clampAmount);
	cameraPos.y = std::clamp(cameraPos.y, pos.y - clampAmount, pos.y + clampAmount);

	cameraOffset = Vector2(round(cameraPos.x) - ((getSize().x) / 2)  ,round(cameraPos.y) - ((getSize().y) / 2 ));
}

void VulkanWindow::setZoom(float x)
{
	zoom = x;
	//SDL_RenderSetScale(renderer, x, x);
}

Vector2 VulkanWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	w = (w & ~1);
	h = (h & ~1);
	
	return Vector2(w, h);
	//return(Vector2(1024, 640));
}

void VulkanWindow::quit() // used before exiting the program
{
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
	SDL_DestroyWindow(window);
	SDL_Quit();
}