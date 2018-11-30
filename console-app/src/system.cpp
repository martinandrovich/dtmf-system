#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <windows.h> //Tjek for fejl senere
#include <memory>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

#include "system.h"

//// Constants ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr int 	KEY_WAIT				= 30;											// Duration between key presses
constexpr int 	KEY_DURATION_MOVE		= 250;											// Duration of a simulated key press (moving)
constexpr int 	KEY_DURATION_PRIMARY	= 25;											// Duration of a simulated key press (primary)

//// Definitions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the CLI
void initCLI()
{
	system("cls");
	std::cout << "Ear Rape Simulator 1.1.0\n\n";
}

// initialize the system
void initSystem(std::string args)
{
	if (args == "server")
	{
		std::cout << "Initializing server ...\n";
		runGame();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		dtmf::node::initializeServer(&executePayload);
		
	}
	else if (args == "client")
	{
		std::cout << "Initializing client ...\n";
		dtmf::node::initializeClient(&executePayload);
		clientWork();
	}
	else
	{
		std::cout << "Please specify type of node to initialize (e.g. init server).\n";
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

// ...
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

// Simulate concurrent global key press for a given duration
void pressKey(int key, int duration, bool parallel)
{
	// define lambda thread
	std::thread t([=]()
	{
		INPUT ip;

		// set up a generic keyboard event
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0; // hardware scan code for key
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		// press the key
		ip.ki.wVk = key; // virtual-key code for the key
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// wait
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));

		// release the key
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));
	});

	// define thread execution method (sequential or concurrent)
	if (parallel)
	{
		t.detach();
	}
	else
	{
		t.join();
	}
}

// ...
void clientWork()
{
	while (true)
	{
		// variables
		int payload		= listenForKey();
		int priority	= 0;

		// escape client work if ESC
		;

		// idenfity message priority (esc>actions>movement)
		if (payload < 5)
		{
			priority = 0;
		}
		else if (payload < 7)
		{
			priority = 1;
		}
		else
		{
			priority = 2;
		}
		
		// send message
		dtmf::node::sendPayload(payload, priority);

		// sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(KEY_WAIT));
	}
}

// ...
void executePayload(int toneId, int clientId)
{
	// WINDOWS INPUT API VALUES

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA

	// keys
	int keyLeft, keyRight, keyUp, keyDown, keyPrimary;

	// log
	std::cout << "##### ID: " << clientId << " | PAYLOAD: " << toneId;

	// determine key values from clientId
	switch (clientId)
	{
	case 1:
		keyLeft		= 0x25;
		keyRight	= 0x27;
		keyUp		= 0x26;
		keyDown		= 0x28;
		keyPrimary	= 0x26;
		break;
	case 2:
		keyLeft		= 0x25;
		keyRight	= 0x27;
		keyUp		= 0x26;
		keyDown		= 0x28;
		keyPrimary	= 0x26;
		break;
	}

	// execute key press in accordance with toneId
	switch (toneId)
	{
	case 1:
		pressKey(keyLeft, KEY_DURATION_MOVE);
		break;
	case 4:
		pressKey(keyRight, KEY_DURATION_MOVE);
		break;
	case 2:
		pressKey(keyUp, KEY_DURATION_MOVE);
		break;
	case 3:
		pressKey(keyDown, KEY_DURATION_MOVE);
		break;
	case 5:
		pressKey(keyPrimary, KEY_DURATION_PRIMARY);
		break;
	}
}

// ...
void runGame() {
	ShellExecute(NULL, "open", "game.exe", NULL, NULL, SW_MAXIMIZE);
	//ShellExecute(0, 0, "https://www.playing-with-fire-game.com/", 0, 0, SW_SHOW);
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
		LOG("\n"

			"init		: initializes type(client/server) \n "
			"exit		: closes console \n "
			"cls		: clears console \n "
			"game		: opens game \n "
			"seq		: plays sequence (toneId1..toneIdn)\n "
			"log		: echoes resived payload \n "
			"qlog		: quiet log \n "
			"slog		: logs the sequence(0-15)\n "
			"glog		: logs the Goertzel response \n "
			"glog_t		: logs the Goertzel response on tread  \n "
			"aglog		: logs the average Goertzel response \n "
			"aglog_t	: logs the average Goertzel response on tread \n "
			"lat		: Returns the latency \n "
			"step		: tests stepwindow \n "
			"sam		: tests sampler \n "
			"gor \n "
			"ggor \n "
			"gvsf		: Returns duration of FFT and Goertzel \n "

		);
	}
}