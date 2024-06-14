#include "console.hpp"

#include "event.hpp"
#include "networking.hpp"

#include <sstream>

#include "gamescene.hpp"
#include "editorscene.hpp"

Console console;

Console::Console()
{

}

Console::~Console()
{

}

void Console::init()
{
	font = window.loadTexture("assets/fonts/font.png");
	output.font = font;
	output.transform = Vector2(32.0f, 0.0f);
	output.lineLength = window.getSize().x / output.fontSize;
	input.uiText.font = font;
	input.tex = -1;
	input.layer = 19;

	panel.tex = -1;
	panel.color = Color4(0.0f, 0.0f, 0.0f, 0.5f);
	panel.transform = Vector2(0.0f, 0.0f);
	panel.size = window.getSize();
	panel.layer = 18;

	framerate.font = font;
	framerate.transform = Vector2(0.0f, 0.0f);

	coordinates.font = font;
	coordinates.transform = Vector2(0.0f, 16.0f);
	coordinates.lineLength = 1024;
}

void Console::log(std::string message) // WIP
{
	data += message;
}

void Console::log(const char* message) // WIP
{
	data += message;
}


void Console::logError(std::string message) // WIP
{

}

void Console::update()
{
	if (Event::KeyDown(SDLK_BACKQUOTE))
	{
		enabled = !enabled;
	}

	if (enabled == true)
	{
		input.transform = Vector2(32, (window.getSize().y - input.size.y) - 32);
		input.size = Vector2(window.getSize().x - 64, 64);
		input.uiText.transform = Vector2(32, window.getSize().y - input.size.y - (input.uiText.fontSize / 2));
		input.uiText.lineLength = input.size.x / output.fontSize;
		input.startTextInput();

		if (Event::TextSubmitted() && input.uiText.getText().length() > 0)
		{
			data += input.uiText.getText() + "\n";
			Console::runCommand(input.uiText.getText());
			input.uiText.setText("");
		}

		output.transform = Vector2(32, ((window.getSize().y - input.size.y) - 64) - output.getLines() * output.fontSize);
		output.lineLength = window.getSize().x / output.fontSize;
		output.setText(data);

		panel.size = window.getSize();
	}
}

void Console::fixedUpdate()
{
	framerate.setText("FPS: " + std::to_string((int)(2000.0 / Time::deltaTime())));
}

void Console::render(OpenGLWindow& window)
{
	if (enabled == true)
	{
		window.render(output, false);
		window.render(input);
		window.render(panel);
	}

	if (showFramerate == true)
	{
		window.render(framerate, false);
	}
	if (showCoordinates == true)
	{
		coordinates.setText("X: " + std::to_string((int)coordinatesPos.x) + "\nY: " + std::to_string((int)coordinatesPos.y));
		window.render(coordinates, false);
	}
}

void Console::runCommand(std::string command)
{
	std::stringstream ss(command);
	std::vector<std::string> v;
	while (std::getline(ss, command, ' '))
	{
		v.push_back(command);
	}

	if (v.size() > 1)
	{
		if (v[0] == "bloom")
		{
			if (v[1] == "true")
			{
				bloom = true;
			}
			else if (v[1] == "false")
			{
				bloom = false;
			}
			else
			{
				Console::log("Cannot set value bloom to: " + v[1] + '\n');
			}
		}
		else if (v[0] == "connect")
		{
			steamSocket.connectIP(v[1]);
		}
		else if (v[0] == "host")
		{
			steamSocket.hostIP(v[1]);
		}
		else if (v[0] == "scene")
		{
			if (v[1] == "game")
			{
				activeScene.openScene(std::make_shared<GameScene>(window));
			}
			else if (v[1] == "editor")
			{
				activeScene.openScene(std::make_shared<EditorScene>(window));
			}
		}
		else if (v[0] == "fpsmax")
		{
			frameCap = std::stoi(v[1]);
		}
		else if (v[0] == "tickrate")
		{
			tickrate = 1000.0 / std::stoi(v[1]);
		}
		else if (v[0] == "ambientlight")
		{
			if (v.size() == 5)
			{
				window.ambientLight = Color4(std::stof(v[1]), std::stof(v[2]), std::stof(v[3]), std::stof(v[4]));
			}
			else
			{
				Console::log("Incorrect number of arguments for command ambientlight\n");
			}
		}
		else if (v[0] == "showfps")
		{
			if (v[1] == "true")
			{
				showFramerate = true;
			}
			else if (v[1] == "false")
			{
				showFramerate = false;
			}
			else
			{
				Console::log("Cannot set value showfps to: " + v[1] + '\n');
			}
		}
		else if (v[0] == "showcoords")
		{
			if (v[1] == "true")
			{
				showCoordinates = true;
			}
			else if (v[1] == "false")
			{
				showCoordinates = false;
			}
			else
			{
				Console::log("Cannot set value showcoords to: " + v[1] + '\n');
			}
		}
		else
		{
			Console::log("Unknown command: " + v[0] + '\n');
		}
	}
	else
	{
		if (v[0] == "quit")
		{
			console.gameRunning = false;
		}
		else if (v[0] == "host")
		{
			steamSocket.hostP2P();
		}
		else if (v[0] == "disconnect")
		{
			steamSocket.disconnect();
		}
		else if (v[0] == "peers")
		{
			console.log("local id " + std::to_string(steamSocket.netConnection) + "\n");
			for (const auto peer : steamSocket.peers)
			{
				console.log("peer id " + std::to_string(peer) + "\n");
			}
		}
		else
		{
			Console::log("Unknown command: " + v[0] + '\n');
		}
	}
}