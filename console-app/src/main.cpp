#pragma once
#include <iostream>
#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

// initialize function
void init()
{
	system("cls");
	std::cout << "Ear Rape Simulator 1.0.3\n\n";
}

// commands map
std::unordered_map<std::string, void(*)(std::string args)> commandMap = 
{
	{ "help",	[](std::string args) { help(args);							} },
	{ "exit",	[](std::string args) { exit(0);								} },
	{ "cls",	[](std::string args) { init();								} },
	{ "test",	[](std::string args) { auto test = dtmf::toolbox::convertAudio("test.wav");	dtmf::toolbox::plotSamples(test);	} },
	{ "log",	[](std::string args) { dtmf::toolbox::testDecoderLog();		} },
	{ "lat",	[](std::string args) { dtmf::toolbox::testLatency();		} },
	{ "step",	[](std::string args) { dtmf::toolbox::testStepWindow2(args);} },
	{ "sam",	[](std::string args) { dtmf::toolbox::testSampler2(); } },
};

// main function
int main()
{
	// init
	init();

	// main loop
	while (true)
	{
		CLI();
	}

	// test functions
	//std::cout << "Testing Decoder Keyboard Presses\n";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	//std::cout << "Testing Audio Conversion and Sample Plot\n";
	//auto test = dtmf::toolbox::convertAudio("test.wav");
	//dtmf::toolbox::plotSamples(test);

	// exit
	return 0;
}
