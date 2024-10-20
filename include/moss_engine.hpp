#pragma once

#include "moss_renderer.hpp"

#include <SDL.h>

#include <memory>
#include <iostream>

class MossEngine
{
public:
	bool init(int argc, char* argv[]);
	void run();
	void cleanup();
private:
	void update(float deltaTime);
	void fixed_update(float deltaTime);

	bool isInitialized = false;
	bool stopRendering = false;
	SDL_Window* window = nullptr;
    std::unique_ptr<Renderer> renderer = nullptr;

	double tickrate = 60.0;
};
