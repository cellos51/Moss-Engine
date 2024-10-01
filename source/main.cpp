#include "moss_engine.hpp"

int main(int argc, char* argv[])
{
    MossEngine engine;

	if (!engine.init())
	{
		std::cerr << "Failed to initialize engine.\n";
		return 1;
	}
	
	engine.run();
	engine.cleanup();

    return 0;
}