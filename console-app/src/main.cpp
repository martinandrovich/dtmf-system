#pragma once
#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

// All essential functionality is located in system.cpp, where main.cpp simply acts
// as an entry point and overview of the system.

// commands map
std::unordered_map<std::string, void(*)(std::string args)> commandMap = 
{
	{ "init",		[](std::string args) { initSystem(args);															} },
	{ "help",		[](std::string args) { help(args);																	} },
	{ "exit",		[](std::string args) { exit(0);																		} },
	{ "cls",		[](std::string args) { initCLI();																	} },
	{ "game",		[](std::string args) { runGame();																	} },
	{ "seq",		[](std::string args) { dtmf::toolbox::playbackSequence(args);										} },
	{ "log",		[](std::string args) { dtmf::toolbox::logDecoder();													} },
	{ "qlog",		[](std::string args) { dtmf::toolbox::logDecoderQuiet();											} },
	{ "slog",		[](std::string args) { dtmf::toolbox::logSequence();												} },
	{ "glog",		[](std::string args) { dtmf::toolbox::logGoertzel(args);											} },
	{ "glog_t",		[](std::string args) { auto t = new std::thread(dtmf::toolbox::logGoertzel, args);					} },
	{ "aglog",		[](std::string args) { auto t = new std::thread(dtmf::toolbox::logGoertzelAverage, args);			} },
	{ "aglog_t",	[](std::string args) { auto t = new std::thread(dtmf::toolbox::logGoertzelAverage, args);			} },
	{ "lat",		[](std::string args) { dtmf::toolbox::testLatency2();												} },
	{ "step",		[](std::string args) { dtmf::toolbox::testStepWindow(args);											} },
	{ "sam",		[](std::string args) { dtmf::toolbox::testSampler();												} },
	{ "gor",		[](std::string args) { dtmf::toolbox::testGoertzel(args);											} },
	{ "ggor",		[](std::string args) { dtmf::toolbox::testGeneratorGoertzel(args);									} },
};

// main function
int main()
{

	std::cout << "Size of short: " << sizeof(signed short) << std::endl;

	// initialize CLI
	initCLI();

	// main loop
	while (true)
	{
		CLI();
	}

	// exit
	return 0;
}
