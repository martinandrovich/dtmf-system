#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "system.h"

//// Definitions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLI()
{
	std::cout << "> ";

	//	receive input
	std::string input;
	std::getline(std::cin, input);

	// check input length
	if (input.size() == 0)
	{
		return;
	}

	// execute command
	executeCommand(input);
}

void executeCommand(std::string input)
{
	std::string cmd = input.substr(0, input.find(' '));
	std::string args = "";

	// check if command exists
	if (commandMap.count(cmd) == 0)
	{
		std::cout << "The command \"" << cmd << "\" is invalid.\n";
		return;
	}

	// check whether command has arguments
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
	
	else
	{
		LOG("That's an invalid fucking argument, you bitch.");
	}
}