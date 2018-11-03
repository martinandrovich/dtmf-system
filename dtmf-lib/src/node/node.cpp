#pragma once
#include "node.h"
#include <thread>

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

	std::vector<State> states;

	void(*actionRecieved)(Action action);



	void nodeThread();

}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

void nodeThread() {
	while (true) {

	}
}

void node::initializeServer(void(*callback)(Action action)) {



	states = {
		State("Base", {
			StateAction()
		}, {
			StateTransition("Base", {
				StateCondition(1,1)
			})
		}),

	};





	//node::worker = std::thread(&nodeThread::thread);




}