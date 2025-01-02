#pragma once

#include "moss_renderer.hpp"

#include <SDL3/SDL.h>

#include <memory>
#include <iostream>

class MossEngine
{
public:
	bool init(int argc, char* argv[]);
	void run();
	void cleanup();
private:
	void fixed_update(float deltaTime);
	void update(float deltaTime);
	void render();

	bool isInitialized = false;
	bool stopRendering = false;
	SDL_Window* window = nullptr;
    std::unique_ptr<Renderer> renderer = nullptr;

	double tickrate = 60.0;
};