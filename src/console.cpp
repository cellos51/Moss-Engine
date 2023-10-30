#include "console.hpp"

#include "event.hpp"

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
			input.uiText.setText("");
		}


		output.transform = Vector2(32, ((window.getSize().y - input.size.y) - 64) - output.getLines() * output.fontSize);
		output.lineLength = window.getSize().x / output.fontSize;
		output.setText(data);

		panel.size = window.getSize();
	}
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