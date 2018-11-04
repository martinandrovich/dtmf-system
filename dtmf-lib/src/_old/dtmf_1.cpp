#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>


#include <thread>

#define WINVER 0x0500

#include "dtmf.h"

#include "node/node.h"
#include "signal/generator.h"
#include "signal/decoder.h"


//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	// Private Methods
	void send(int msg); // msg struct
	void process(int toneID);
	void recieved(Message msg);

	int  testCurrentState();
	bool testTransition(int transition);
	bool testCondition(int condition);

	// Private Members
	State* currentState;
	int clientID;

	std::vector<State> states;

	void(*actionRecieved)(Action action);

	int dir = 2;

	void nodeThread();

}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

void nodeThread() {
	while (true) {

	}
}

void dtmf::initializeServer() {



	states = {
		State("Base", {
			StateAction()
		}, {
			new StateTransition("Base", {
				new StateCondition([] { return dir > 3; })
			})
		}),

	};

	currentState = &states[0];






	//node::worker = std::thread(&nodeThread::thread);




}


void dtmf::test()
{
	
	for (auto t : currentState->transitions)
	{
		for (auto c : t->conditions)
		{

			dir = 4;
			if (c->result()) // = true
				; // change state if true

			dir = 2;
			if (*c) // = false
				; // change state if true
		}
	}
}