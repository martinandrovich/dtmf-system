#pragma once
#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

// All essential functionality is located in system.cpp, where main.cpp simply acts
// as an entry point and overview of the system.

// commands map
std::unordered_map<std::string, void(*)(std::string args)> commandMap = 
{
	{ "init",	[](std::string args) { initSystem(args);													} },
	{ "help",	[](std::string args) { help(args);															} },
	{ "exit",	[](std::string args) { exit(0);																} },
	{ "cls",	[](std::string args) { initCLI();															} },
	{ "log",	[](std::string args) { dtmf::toolbox::logDecoder();											} },
	{ "seq",	[](std::string args) { dtmf::toolbox::playbackSequence(args);								} },
	{ "glog",	[](std::string args) { dtmf::toolbox::logGoertzel(args);									} },
	{ "glogt",	[](std::string args) { auto glog = new std::thread(dtmf::toolbox::logGoertzel, args);		} },
	{ "lat",	[](std::string args) { dtmf::toolbox::testLatency();										} },
	{ "step",	[](std::string args) { dtmf::toolbox::testStepWindow2(args);								} },
	{ "sam",	[](std::string args) { dtmf::toolbox::testSampler2();										} },
	{ "gor",	[](std::string args) { dtmf::toolbox::testGeneratorGoertzel(args);							} },
};

// main function
int main()
{
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
