#include <iostream>
#include <string>
#include <functional>
#include <map>
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
std::string findCommand(std::string input) 
{
	
	if (input.find(' ') == -1) 
	{
		return input;
	}
	
	std::string command;
	for (int i = 0; i < input.length(); i++) 
	{
		if (input[i] == ' ') 
		{
			command = input.substr(0, i);
			return command;
		}
	}
}

std::string findArgument(std::string input) 
{
	std::string argument;
	for (int i = 0; i < input.length(); i++) 
	{
		if (input[i] == ' ') 
		{
			argument = input.substr(i + 1, (input.size() - i));
			return argument;
		}
	}
}

void executeCommand(std::string input) 
{
	
	std::string cmd = findCommand(input);
	std::string arg = "";

	
	if (input.find(' ') != -1) 
	{
		arg = findArgument(input);
		commandMap[cmd](arg);
	}
	else 
	{
		commandMap[cmd](arg);
	}
}

void help(std::string args) 
{
	if (args == "help") 
	{
		LOG("[HELP MESSAGE]: This function \"help\" informs you about the given function using the following format: help [Some function].");
	}

	if (args == "exit") 
	{
		LOG("[HELP MESSAGE]: This function \"exit\" exits the console.");
	}

	if (args == "test") 
	{
		LOG("[HELP MESSAGE]: This is just a test function......yeah......no actual functionality.")
	}
		
}

void end() 
{
	exit(0);
}

//void init() {
//
//	char ID;
//	std::cout << "Er jeg server eller klient? [SPACE]/g \n";
//	char sok = std::getchar();
//	if (sok == ' ') {
//		std::cout << "Server valgt \n";
//		ID = 'S';
//	}
//	else {
//		std::cout << "Klient valgt \n";
//		ID = 'K';
//	}
//
//	switch (ID)
//	{
//	case 'S': {
//		char asd;
//		std::cout << "Vil du tilkoble klienter, y/n? \n";
//			std::cin >> asd;
//		if (asd == 'y') {
//			dtmf::testGenerator();
//		}
//	}
//	case 'K': {
//		while (true)
//			return;
//	}
//	}
//}

int main()
{	
	//	Map definition.
	commandMap = 
	{
		{"test",[=](std::string args) {LOG("yo yo this is a test")}},
		{"help",[=](std::string args) {help(args); }},
		{"exit",[=](std::string args) {end(); }}
	};

	LOG("DTMF project - CLI v1.0");

	while (true) 
	{

		std::cout << "> ";

		std::string input;

		//	Receive input.
		std::getline(std::cin, input);
		std::string cmd = findCommand(input);

		if (input.size() == 0)
		{
			continue;
		}

		//	Check if command exists.
		if (commandMap.count(cmd) == 0)
		{
			std::cout << "The command " << cmd << " is invalid." << std::endl;
			return 0;
		}

		executeCommand(input);
	}

	std::cin.get();
	return 0;
}