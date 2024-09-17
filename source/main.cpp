#include "moss_engine.hpp"

int main(int argc, char* argv[])
{
    MossEngine engine;

	engine.init();
	engine.run();
	engine.cleanup();

    return 0;
}