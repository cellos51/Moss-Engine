#include "moss_engine.hpp"

int main(int argc, char* argv[])
{
    MossEngine engine;

	engine.init(argc, argv);
	engine.run();
	engine.cleanup();

    return 0;
}