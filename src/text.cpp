#include "text.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

Text::Text()
{

}

void Text::setText(std::string data)
{
	if (data == message && lineLength == oldLength)
	{
		return;
	}

	message = data;
	characters.clear();

	int i = 0;
	int currentLine = 0;
	for(char& c : data)
	{
		if (c == '\n')
		{
			currentLine++;
			i = 0;
		}
		else
		{
			currentLine += ((i % lineLength) == 0) ? 1 : 0;

			// setup
			Entity character(Vector2(transform.x, transform.y - fontSize), Vector2(fontSize, fontSize)); // for some fucking reason i have to do this dumb fucking offset or else the fucking position is slightly off

			character.luminosity = luminosity;

			character.transform.x = character.transform.x + ((i % lineLength) * fontSize);
			character.transform.y += fontSize * currentLine;

			character.tex = font;

			character.layer = 20; // needs to be dymanic but fine for now

			//text selection... FOR EVERY CHARACTER IN THE FONT LIKE BRUH
			switch (c)
			{
			case 'A':
			{
				character.texturePos = SDL_Rect{ 0, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'B':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'C':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'D':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'E':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'F':
			{
				character.texturePos = SDL_Rect{ 5 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'G':
			{
				character.texturePos = SDL_Rect{ 6 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'H':
			{
				character.texturePos = SDL_Rect{ 7 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'I':
			{
				character.texturePos = SDL_Rect{ 8 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'J':
			{
				character.texturePos = SDL_Rect{ 9 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'K':
			{
				character.texturePos = SDL_Rect{ 10 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'L':
			{
				character.texturePos = SDL_Rect{ 11 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'M':
			{
				character.texturePos = SDL_Rect{ 12 * fontSize, 5 * fontSize, fontSize, fontSize };
				break;
			}
			case 'N':
			{
				character.texturePos = SDL_Rect{ 0 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'O':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'P':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'Q':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'R':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'S':
			{
				character.texturePos = SDL_Rect{ 5 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'T':
			{
				character.texturePos = SDL_Rect{ 6 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'U':
			{
				character.texturePos = SDL_Rect{ 7 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'V':
			{
				character.texturePos = SDL_Rect{ 8 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'W':
			{
				character.texturePos = SDL_Rect{ 9 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'X':
			{
				character.texturePos = SDL_Rect{ 10 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'Y':
			{
				character.texturePos = SDL_Rect{ 11 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'Z':
			{
				character.texturePos = SDL_Rect{ 12 * fontSize, 4 * fontSize, fontSize, fontSize };
				break;
			}
			case 'a':
			{
				character.texturePos = SDL_Rect{ 0, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'b':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'c':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'd':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'e':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'f':
			{
				character.texturePos = SDL_Rect{ 5 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'g':
			{
				character.texturePos = SDL_Rect{ 6 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'h':
			{
				character.texturePos = SDL_Rect{ 7 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'i':
			{
				character.texturePos = SDL_Rect{ 8 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'j':
			{
				character.texturePos = SDL_Rect{ 9 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'k':
			{
				character.texturePos = SDL_Rect{ 10 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'l':
			{
				character.texturePos = SDL_Rect{ 11 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'm':
			{
				character.texturePos = SDL_Rect{ 12 * fontSize, 3 * fontSize, fontSize, fontSize };
				break;
			}
			case 'n':
			{
				character.texturePos = SDL_Rect{ 0 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'o':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'p':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'q':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'r':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 's':
			{
				character.texturePos = SDL_Rect{ 5 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 't':
			{
				character.texturePos = SDL_Rect{ 6 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'u':
			{
				character.texturePos = SDL_Rect{ 7 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'v':
			{
				character.texturePos = SDL_Rect{ 8 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'w':
			{
				character.texturePos = SDL_Rect{ 9 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'x':
			{
				character.texturePos = SDL_Rect{ 10 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'y':
			{
				character.texturePos = SDL_Rect{ 11 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case 'z':
			{
				character.texturePos = SDL_Rect{ 12 * fontSize, 2 * fontSize, fontSize, fontSize };
				break;
			}
			case '0':
			{
				character.texturePos = SDL_Rect{ 0 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '1':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '2':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '3':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '4':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '5':
			{
				character.texturePos = SDL_Rect{ 5 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '6':
			{
				character.texturePos = SDL_Rect{ 6 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '7':
			{
				character.texturePos = SDL_Rect{ 7 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '8':
			{
				character.texturePos = SDL_Rect{ 8 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '9':
			{
				character.texturePos = SDL_Rect{ 9 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '.':
			{
				character.texturePos = SDL_Rect{ 10 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case '!':
			{
				character.texturePos = SDL_Rect{ 11 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case ',':
			{
				character.texturePos = SDL_Rect{ 12 * fontSize, 1 * fontSize, fontSize, fontSize };
				break;
			}
			case ';':
			{
				character.texturePos = SDL_Rect{ 0, 0, fontSize, fontSize };
				break;
			}
			case ':':
			{
				character.texturePos = SDL_Rect{ 1 * fontSize, 0, fontSize, fontSize };
				break;
			}
			case '(':
			{
				character.texturePos = SDL_Rect{ 2 * fontSize, 0, fontSize, fontSize };
				break;
			}
			case ')':
			{
				character.texturePos = SDL_Rect{ 3 * fontSize, 0, fontSize, fontSize };
				break;
			}
			case ' ':
			{
				character.texturePos = SDL_Rect{ 4 * fontSize, 0, fontSize, fontSize };
				break;
			}
			}

			characters.push_back(character);
			i++;
		}
	}

	lines = currentLine;
	oldLength = lineLength;
}

void Text::setTextC(char *data)
{
	std::string str(data);
	message = str;
}

std::string Text::getText()
{
	return message;
}

unsigned int Text::getLines()
{
	return lines;
}