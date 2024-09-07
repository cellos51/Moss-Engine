#include "window.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) 
{
    try
    {
        Window window(800, 600, "Moss Engine", SDL_WINDOW_VULKAN);

        while (true)
        {
            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    break;
                }
            }

            window.render();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}