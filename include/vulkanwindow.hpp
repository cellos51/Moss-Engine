#pragma once

#include <SDl2/SDL.h>
#include <SDL2/SDL_Vulkan.h>
#include <vulkan/vulkan.h>

#include "math.hpp"

class VulkanWindow
{
public:
	void create(const char* p_title, int p_w, int p_h);
	//unsigned int loadTexture(const char* p_filePath);
	//void clear();
	//void render(Entity& p_ent, bool cam);
	//void render(Text& p_text, bool cam);
	//void render(ui& p_ui);
	//void render(Light& p_light);
	//void display();
	void quit();
	void camera(Vector2 pos);
	void setZoom(float x);
	Vector2 getSize();
	Vector2 cameraPos = Vector2(0,0);
	//Color4 ambientLight = Color4(0.5,0.5,0.5,1);
private:
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();
	bool checkValidationLayerSupport();
	VkQueue graphicsQueue;
	VkSurfaceKHR surface;
	VkDevice device;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	Vector2 cameraOffset = Vector2(0,0);
	SDL_Window* window;
	float zoom = 1;
	int clampAmount = 5;
	float lerpAmount = 0.005;
	//SDL_GLContext context;
};