#pragma once

#include "moss_renderer.hpp"

#include <SDL.h>

#include <memory>

class MossEngine
{
public:
	static MossEngine& Get();
	void init();
	void cleanup();
	void draw();
	void run();
private:
	bool _isInitialized{ false };
	bool stop_rendering{ false };
	SDL_Window* _window{ nullptr };
    std::unique_ptr<MossRenderer> _renderer{ nullptr };
};
