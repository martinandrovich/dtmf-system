#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>

#include <map>



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


	void testCurrentState();
	int getStateId(dtmf::StateTransition& transition);
	bool testTransition(dtmf::StateTransition& transition);

	void runStateActions();



	void checkAction();
	void checkMessage();
	void checkTimeOut();

	void checkTriggers();




	void stateMachineThread();

	// Private Members
	int currentState=0;
	bool isQuickState = false;
	int currentErrorState; //state to return to upon error signal
	int clientID=-1;
	int var1=32, var2=45, var3=112, var4=56; //random access variables for state machine
	

	
	
	


	std::thread	stateMachine;

	std::mutex messageLock;
	bool newMessageFlag, newActionFlag;
	Action::actions currentAction;
	Message currentMessage;
	int timeoutTimer;
	int oldToneId;
	bool hasRecievedDirID;


	std::vector<State> states;

	void(*actionRecieved)(Action action);



	

}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////




void dtmf::process(int toneID)
{
	messageLock.lock();
	if (toneID == 15) {
		hasRecievedDirID = false;

	}
	else if (hasRecievedDirID == false) {
		
		if (isQuickState) {
			currentMessage = Message(toneID);
			newMessageFlag = true;
		}
		else {

			oldToneId = toneID;
			hasRecievedDirID = true;
		}
	}
	else {
		currentMessage = Message(oldToneId, toneID);
		hasRecievedDirID = false;
		newMessageFlag = true;
	}
	messageLock.unlock();
}

void dtmf::actionSend(Action::actions action)
{
	messageLock.lock();
	if (newActionFlag) {
		currentAction = action;
	}

	newActionFlag = true;
	messageLock.unlock();
}



void dtmf::testCurrentState()
{
	for (auto& transition : states[currentState].transitions) {
		if (testTransition(transition)) {
			currentState=getStateId(transition);
			isQuickState = states[currentState].isQuickState;
			runStateActions();
			return;
		}
	}
}

int dtmf::getStateId(dtmf::StateTransition& transition)
{
	std::cout << "changing state to "<<transition.targetName << "\n";
	if (transition.targetId != -1) {
		return transition.targetId;
	}
	for (int i = 0; i < states.size(); i++) {
		if (states[i].name == transition.targetName) {
			
			transition.targetId=i;
			return i;
		}
	}
	return -1;
}


bool dtmf::testTransition(dtmf::StateTransition& transition)
{
	for (const auto& condition : transition.conditions) {
		if (!condition) {
			return false; //if any condition is false return false;
		}
	}
	return true; //else return true
}



void dtmf::runStateActions()
{
	for (auto action : states[currentState].actions) {
		action.function();
	}
	testCurrentState();
}


void dtmf::checkAction() {
	messageLock.lock();
	if (newActionFlag) {
		testCurrentState();
		newActionFlag = false;
		currentAction = Action::null;
	}

	messageLock.unlock();
}
void dtmf::checkMessage() {
	messageLock.lock();
	if (newMessageFlag) {
		testCurrentState();

		newMessageFlag = false;
		currentMessage = Message();
	}

	messageLock.unlock();
}
void dtmf::checkTimeOut() {

}

void dtmf::checkTriggers() {
	
	checkAction();
	checkMessage();
	checkTimeOut();


}


void dtmf::stateMachineThread() {
	while (true) {

		checkTriggers();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		timeoutTimer += 1;
	}
}



void dtmf::initializeServer(void(*actionRecieved)(Action action)) {

	states = {
		State("Base", {
			
		}, {
			StateTransition("Two", {
				StateCondition([] { return var1==5; })
			}),
			
		}),
		State("Two", {
			
		}, {
			StateTransition("Three", {
				
			}),
			
		}),
		State("Three", {
			StateAction([] { var1--; })
		}, {
			StateTransition("Base", {
				StateCondition([] { return var1 == 3; })
			}),
			
		}),

	};
	



	
	std::cout << "State: "<<states[currentState].name << "\n";
	var1 = 5;
	testCurrentState();
	std::cout << "State: " << states[currentState].name << "\n";
	testCurrentState();
	std::cout << "State: " << states[currentState].name << "\n";
	var1--;
	testCurrentState();
	std::cout << "State: " << states[currentState].name << "\n";
	var1--;
	testCurrentState();
	std::cout << "State: " << states[currentState].name << "\n";



	std::cout << states[0].transitions[0].targetId << "\n";
	std::cout << states[1].transitions[0].targetId << "\n";
	std::cout << states[2].transitions[0].targetId << "\n";

	//dtmf::actionRecieved = actionRecieved;
	//dtmf::stateMachine = std::thread(&dtmf::stateMachineThread);




}