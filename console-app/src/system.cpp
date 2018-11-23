#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <windows.h> //Tjek for fejl senere

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

//// Constants __//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr int 	KEY_WAIT = 50;											// Duration between key presses

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
	if (args == "server")
	{
		std::cout << "Initializing server ...\n";
		dtmf::node::initializeServer(&someFunction);
	}
	else if (args == "client")
	{
		std::cout << "Initializing client ...\n";
		dtmf::node::initializeClient(&someFunction);
		clientWork();
	}
}

// ...
void CLI()
{
	// show >
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::cout << "> ";

	//	receive input
	std::string input;
	std::getline(std::cin, input);

	// check input length
	if (input.size() == 0)
	{
		return;
	}

	// execute input command
	executeCommand(input);
}
int listenForKey()
{
	int key = 0;
	if (GetKeyState('S') < 0)
		LOG("s down")
		if (GetKeyState(VK_LEFT) < 0)
			key = 1;
	if (GetKeyState(VK_UP) < 0)
		key = 2;
	if (GetKeyState(VK_DOWN) < 0)
		key = 3;
	if (GetKeyState(VK_RIGHT) < 0)
		key = 4;
	if (GetKeyState(VK_SPACE) < 0)
		key = 5;
	if (GetKeyState(VK_CONTROL) < 0)
		key = 6;
	if (GetKeyState(VK_ESCAPE) < 0)
		key = 7;

	return key;
}
void clientWork()
{
	while (true)
	{
		//Implementering af prioritetsliste: esc>actions>movement
		int payload = listenForKey();
		int priority;
		if (payload < 5)
			priority = 0;
		else if (payload < 7)
			priority = 1;
		else priority = 2;
		dtmf::node::sendPayload(payload, priority);
		//if (keydown != 0)
			//LOG(keydown);
		std::this_thread::sleep_for(std::chrono::milliseconds(KEY_WAIT));
	}
}
void	runGame() {
	ShellExecute(0, 0, "https://www.playing-with-fire-game.com/", 0, 0, SW_SHOW);
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