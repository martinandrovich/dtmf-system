#pragma once
#include <vector>

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace node
{

	// Public Constructs
	struct Action
	{
		int clientID;
		enum actions { null, up, down, left, right, primary, secondary, menu };
	};

	struct Message
	{
		Message(int address, int command) : direction(address / 8), id(address & 7), address(address), command(command) {};
		Message(int direction, int id, int command) : direction(direction), id(id), address(direction * 8 + id), command(command) {};

		int command;
		int direction;
		int id;
		int address;
	};

	// Public Methods
	void actionSend(Action::actions action);
};