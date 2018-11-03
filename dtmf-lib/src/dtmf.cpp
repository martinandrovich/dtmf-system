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

void dtmf::initializeServer(void(*callback)(Action action)) {



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