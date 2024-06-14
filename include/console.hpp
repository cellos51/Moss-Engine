#pragma once

#include "scene.hpp"

class Console // this bad boy needs a lot of work but'll be fine for now as i'm the only person working on it
{
public:
	Console();
	~Console();
	bool enabled = false;
	void init();
	void log(std::string message);
	void log(const char* message);
	void logError(std::string message);
	void update();
	void fixedUpdate();
	void render(OpenGLWindow& window);

	// convars
	bool gameRunning = true;
	bool bloom = true;
	bool showFramerate = false;
	bool showCoordinates = false;
	Vector2 coordinatesPos = Vector2(0, 0);
	int frameCap = 0;
	double tickrate = 1000.0 / 60.0;
private:
	void runCommand(std::string command);

	unsigned int font = -1;
	unsigned int maxLines = 5;
	Text output;
	ui::TextInput input;
	std::string data = "";
	ui::Panel panel;

	Text framerate;
	Text coordinates;
};

extern Console console;