#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

//// Definitions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initialize the CLI
void initCLI()
{
	system("cls");
	std::cout << "Ear Rape Simulator 1.0.5\n\n";
}

// initialize the System
void initSystem(std::string args)
{
	dtmf::node::initializeServer(&someFunction);
}

// ...
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

// ...
void executeCommand(std::string input)
{
	// parse cmd
	std::string cmd = input.substr(0, input.find(' '));
	std::string args = "";

	// check if command exists
	if (commandMap.count(cmd) == 0)
	{
		std::cout << "The command \"" << cmd << "\" is invalid.\n";
		return;
	}

	// check whether command has arguments; parse args if true
	if (input.find(' ') != -1)
	{
		args = input.substr(cmd.length() + 1);
	}
	
	// call appropriate function from map
	commandMap[cmd](args);
}

// ...
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

// ...
void someFunction(int payload, int id)
{
	LOG("HELLO!");
	LOG(payload);
	LOG(id);
}