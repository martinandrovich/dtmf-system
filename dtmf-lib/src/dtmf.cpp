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


	int  testCurrentState();
	int getTransitionId(dtmf::StateTransition transition);
	bool testTransition(dtmf::StateTransition transition);
	bool testCondition(dtmf::StateCondition condition);

	
	int getIndexedValue(dtmf::indexValues index);
	void setIndexedValue(dtmf::indexValues index, int value);



	void checkAction();
	void checkMessage();
	void checkTimeOut();

	void checkTriggers();




	void stateMachineThread();

	// Private Members
	int currentState=0;
	int currentErrorState; //state to return to upon error signal
	int clientID=-1;
	int var1=32, var2=45, var3=112, var4=56; //random access variables for state machine
	

	
	
	std::map<indexValues, int*> mappedValues = { 
		{indexValues::clientID,&clientID},
		{indexValues::currentState,&currentState},
		{indexValues::var1,&var1},
		{indexValues::var2,&var2},
		{indexValues::var3,&var3},
		{indexValues::var4,&var4}
	};


	std::thread	stateMachine;

	std::mutex messageLock;
	std::mutex actionLock;
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
		oldToneId = toneID;
		hasRecievedDirID = true;
	}
	else {
		currentMessage = Message(oldToneId, toneID);
		hasRecievedDirID = false;
		newMessageFlag = true;
	}
	messageLock.unlock();



}



int dtmf::testCurrentState()
{
	for (auto transition : states[currentState].transitions) {
		if (testTransition(transition)) {
			return getTransitionId(transition);
		}
	}
	return -1;
}

int dtmf::getTransitionId(dtmf::StateTransition transition)
{
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


bool dtmf::testTransition(dtmf::StateTransition transition)
{
	for (auto condition : transition.conditions) {
		if (!testCondition(condition)) {
			return false; //if any condition is false return false;
		}
	}
	return true; //else return true
}

bool dtmf::testCondition(dtmf::StateCondition condition)
{
	return false;
}

int dtmf::getIndexedValue(dtmf::indexValues index)
{
	if (index <= dtmf::indexValues::C15) {
		return (int)index;
	}
	if (index <= dtmf::indexValues::var4) {
		return *dtmf::mappedValues[index];
	}
	switch (index)
	{
	case dtmf::indexValues::action:
		return (int)currentAction;
	case dtmf::indexValues::direction:
		return (int)currentMessage.direction;
	case dtmf::indexValues::id:
		return (int)currentMessage.id;
	case dtmf::indexValues::data:
		return (int)currentMessage.command;
	case dtmf::indexValues::randomValue:
		return rand() % 14; //generate random nuber from 0-13 (allowed numbers, 14 is error message, 15 is syncronization message)
	default:
		return -1;
	}
}
void dtmf::setIndexedValue(dtmf::indexValues index, int value)
{
	if (index <= dtmf::indexValues::C15) {
		return;
	}
	if (index <= dtmf::indexValues::var4) {
		*dtmf::mappedValues[index]=value;
	}
	switch (index)
	{
	case dtmf::indexValues::action:
		currentAction = (dtmf::Action::actions)value;
		return;
	case dtmf::indexValues::direction:
		currentMessage.direction = value;
		currentMessage.address = currentMessage.direction * 8 + currentMessage.id;
		return;
	case dtmf::indexValues::id:
		currentMessage.id = value;
		currentMessage.address = currentMessage.direction * 8 + currentMessage.id;
		return;
	case dtmf::indexValues::data:
		currentMessage.command = value;
		return;
	
	default:
		return;
	}
}






void dtmf::checkAction() {
	actionLock.lock();
	if (newActionFlag) {
		testCurrentState();
		newActionFlag = false;
		currentAction = Action::null;
	}

	actionLock.unlock();
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


		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		timeoutTimer += 1;
	}
}

void dtmf::actionSend(Action::actions action)
{
	actionLock.lock();
	if (newActionFlag) {
		currentAction = action;
	}

	newActionFlag = true;
	actionLock.unlock();
}

void dtmf::initializeServer(void(*actionRecieved)(Action action)) {


	std::cout << getIndexedValue(indexValues::id) << "\n";



	states = {
		State("Base", {
			StateAction()
		}, {
			StateTransition("Base", {
				StateCondition(6,1)
			}),
			StateTransition("Base", {
				StateCondition(8,1)
			}),
			StateTransition("Base", {
				StateCondition(11,1)
			})
		}),

	};

	std::cout << states[0].transitions[2].conditions[0].subject<< "\n";


	dtmf::actionRecieved = actionRecieved;
	dtmf::stateMachine = std::thread(&dtmf::stateMachineThread);




}