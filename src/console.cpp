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

void Console::init(OpenGLWindow& window)
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

void Console::update(OpenGLWindow& window)
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

void Console::fixedUpdate(OpenGLWindow& window)
{

}

void Console::render(OpenGLWindow& window)
{
	if (enabled == true)
	{
		window.render(output, false);
		window.render(input);
		window.render(panel);
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
	}
}