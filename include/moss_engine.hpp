#pragma once

#include "moss_renderer.hpp"

#include <SDL.h>

#include <memory>
#include <iostream>

class MossEngine
{
public:
	static MossEngine& Get();
	bool init();
	void run();
	void cleanup();
private:
	bool isInitialized = false;
	bool stopRendering = false;
	SDL_Window* window = nullptr;
    std::unique_ptr<MossRenderer> renderer = nullptr;
};
