#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#define LOG(x) std::cout << x << std::endl;

//	Map declaration.
std::unordered_map<std::string, std::function<void(std::string args)>> commandMap;

//	Function declarations.
void			end();
void			executeCommand(std::string input);
void			help(std::string args);
std::string		findCommand(std::string input);
std::string		findArgument(std::string input);

//	Function definitions.
void CLI()
{
	std::cout << "> ";

	std::string input;

	//	receive input
	std::getline(std::cin, input);
	std::string cmd = findCommand(input);

	if (input.size() == 0)
	{
		continue;
	}

	//	check if command exists
	if (commandMap.count(cmd) == 0)
	{
		std::cout << "The command " << cmd << " is invalid." << std::endl;
		return 0;
	}

	executeCommand(input);
}

void executeCommand(std::string input)
{
	std::string cmd		= input.substr(0, input.find(' '));
	std::string args 	= "";

	if (input.find(' ') != -1)
	{
		args = input.substr(cmd.length() + 1);
		commandMap[cmd](args);
	}
	else
	{
		commandMap[cmd](args);
	}
}

void help(std::string args)
{
	if (args == "help")
	{
		LOG("[HELP MESSAGE]: This function \"help\" informs you about the given function using the following format: help [Some function].");
	}

	else if (args == "exit")
	{
		LOG("[HELP MESSAGE]: This function \"exit\" exits the console.");
	}

	else if (args == "test")
	{
		LOG("[HELP MESSAGE]: This is just a test function......yeah......no actual functionality.");
	}
}

int main()
{

	// init
	std::cout << "Ear Rape Simulator 1.1\n\n";

	//	commands map definiton
	commandMap =
	{
		{ "test", [](std::string args) { LOG("yo yo this is a test") } },
		{ "help", [](std::string args) { help(args); } },
		{ "exit", [](std::string args) { exit(0); } }
	};

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

	// stall & exit
	std::cin.get();
	return 0;
}
