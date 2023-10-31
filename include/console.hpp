#pragma once

#include "scene.hpp"

class Console // this bad boy needs a lot of work but'll be fine for now as i'm the only person working on it
{
public:
	Console();
	~Console();
	bool enabled = false;
	void init(OpenGLWindow& window);
	void log(std::string message);
	void log(const char* message);
	void logError(std::string message);
	void update(OpenGLWindow& window);
	void render(OpenGLWindow& window);

	// convars
	bool gameRunning = true;
	bool bloom = true;
private:
	void runCommand(std::string command);

	unsigned int font = -1;
	unsigned int maxLines = 5;
	Text output;
	ui::TextInput input;
	std::string data = "";
	ui::Panel panel;
};

extern Console console;