#pragma once
#include "node.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace node
{
	// Private Methods
	void send(int msg); // msg struct
	void process(int toneID);
	void recieved(Message msg);

	int  testCurrentState();
	bool testTransition(int transition);
	bool testCondition(int condition);

	// Private Members
	int currentState;
	int clientID;

	std::vector<int> states;

	void(*actionRecieved)(Action action);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

;