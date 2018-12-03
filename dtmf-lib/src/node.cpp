#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>

#include "dtmf/node.h"
#include "signal/generator.h"
#include "signal/decoder.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	namespace node {

		using namespace std::chrono;

		// Private Members
		std::vector<State>	states;
		StateTransition		errorTransition;
		
		std::thread			stateMachine;
		std::mutex			messageMutex;

		bool				isInitialized = false;
		bool				isServer;
		bool				isQuickState = false;

		void(*callbackFunction)(int payload, int id);

		high_resolution_clock				clock;
		time_point<high_resolution_clock>	timestamp;

		bool				newMessageFlag, newActionFlag;
		int					clientID = -1;
		int					numClients = 0;
		int					idCounter = 1;

		int					currentState = 0;
		int					currentErrorState; //state to return to upon error signal
		int					currentAction;
		int					currentActionPriority;
		Message				currentMessage;

		int					timeoutTimer;
		int					oldToneId;
		bool				hasRecievedDirID;
				
		int					var1 = 32, var2 = 45, var3 = 112, var4 = 56; //random access variables for state machine

		// Private Methods
		void send(Message msg); // msg struct
		void sync();
		void process(uint toneID);
		void recieved(Message msg);
		
		void testCurrentState();
		int  getStateId(StateTransition& transition);
		bool testTransition(StateTransition& transition);

		void runStateActions();

		void checkAction();
		void checkMessage();
		void checkIsTimeout();
		void checkTriggers();
		bool checkTimeout();

		void stateMachineThread();
	}
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// ...
void dtmf::node::send(Message msg)
{
	std::vector<int> sequence = { msg.address, msg.command };
	generator::playbackSequence(sequence);

	newActionFlag = false;
	currentAction = null;
	currentActionPriority = 0;
}
void dtmf::node::sync()
{
	std::vector<int> sequence = {15 };
	generator::playbackSequence(sequence);

}
// ...
void dtmf::node::process(uint toneID)
{
	messageMutex.lock();
	node::timestamp = node::clock.now();
	std::cout << "Got tone [" << toneID << "]\n";
		if (toneID == 15) {
			hasRecievedDirID = false;
		}
		else if (hasRecievedDirID == false)
		{
			if (isQuickState)
			{
				currentMessage = Message(toneID);
				newMessageFlag = true;
			}
			else
			{
				oldToneId = toneID;
				hasRecievedDirID = true;
			}
		}
		else
		{
			currentMessage = Message(oldToneId, toneID);
			hasRecievedDirID = false;
			newMessageFlag = true;
		}

	messageMutex.unlock();
}

// ...
void dtmf::node::sendPayload(int action, int priority)
{
	messageMutex.lock();

		if (currentActionPriority <= priority) {
			currentAction = action;
			currentActionPriority = priority;
		}

		newActionFlag = true;

	messageMutex.unlock();
}

// ...
bool dtmf::node::payloadReady()
{
	return newActionFlag;
}

// ...
void dtmf::node::testCurrentState()
{
	if (testTransition(errorTransition))
	{
		currentState = getStateId(errorTransition);
		isQuickState = states[currentState].isQuickState;
		//timeoutTimer = 0;
		node::timestamp = node::clock.now();
		runStateActions();
		return;
	}
	for (auto& transition : states[currentState].transitions)
	{
		if (testTransition(transition))
		{
			currentState = getStateId(transition);
			isQuickState = states[currentState].isQuickState;
			//timeoutTimer = 0;
			node::timestamp = node::clock.now();
			runStateActions();
			return;
		}
	}
}

// ...
int dtmf::node::getStateId(StateTransition& transition)
{
	std::cout << "changing state to " << transition.targetName << "\n";

	if (transition.targetId != -1)
	{
		return transition.targetId;
	}

	for (int i = 0; i < states.size(); i++)
	{
		if (states[i].name == transition.targetName)
		{
			transition.targetId = i;
			return i;
		}
	}

	return -1;
}

// ...
bool dtmf::node::testTransition(StateTransition& transition)
{
	for (const auto& condition : transition.conditions)
	{
		if (!condition)
		{
			return false; //if any condition is false return false;
		}
	}

	return true; //else return true
}

// ...
void dtmf::node::runStateActions()
{
	for (auto action : states[currentState].actions)
	{
		action.function();
	}
	currentMessage = Message();
	newMessageFlag = false;
	testCurrentState();
}

// ...
void dtmf::node::checkAction()
{
	messageMutex.lock();

		if (newActionFlag)
		{
			testCurrentState();
		}

	messageMutex.unlock();
}

// ...
void dtmf::node::checkMessage()
{
	messageMutex.lock();

		if (newMessageFlag)
		{
			testCurrentState();

			newMessageFlag = false;
			currentMessage = Message();
		}

	messageMutex.unlock();
}

// ...
void dtmf::node::checkIsTimeout()
{
	messageMutex.lock();
	if (checkTimeout()) {
		testCurrentState();
	}
	messageMutex.unlock();
}

// ...
void dtmf::node::checkTriggers()
{
	checkAction();
	checkMessage();
	checkIsTimeout();
}

// ...
bool dtmf::node::checkTimeout()
{
	return ((int)static_cast<duration<double, std::milli>>(node::clock.now() - node::timestamp).count() > TIMEOUT);
}

// ...
void dtmf::node::stateMachineThread()
{
	while (true)
	{
		checkTriggers();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		timeoutTimer += 1;
	}
}

// ...
void dtmf::node::initializeClient(void(*callback)(int payload, int id))
{
	if (isInitialized) {
		return;
	}

	isInitialized = true;
	isServer = false;

	callbackFunction = callback;

	// timestamp
	node::timestamp = node::clock.now();

	// state definitions for CLIENT node

	errorTransition = StateTransition("error",
		{
			StateCondition([] { return currentMessage.command == 14; }),
		});

	states = {
		State("start",
			{
			},{
				StateTransition("connect",
					{
						StateCondition([] { return currentAction != 0; }),
						StateCondition([] { return newActionFlag; })
					}),

			}),
		State("error",
			{
				StateAction([] { currentState = currentErrorState; })
			},{
				

			}),
		State("connect",
			{
				StateAction([] { sync(); }),
				StateAction([] { var1 = currentAction; }),
				StateAction([] { send(Message((int)isServer, 0, var1)); })
			},{
				StateTransition("setId",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] { return currentMessage.command == var1; }),
						StateCondition([] { return currentMessage.id != 0; })
					}),
				StateTransition("start",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] { return currentMessage.command != var1; })
					}),
				StateTransition("start",
					{
						StateCondition([] { return checkTimeout(); })
					}),

			}),
		State("setId",
			{
				StateAction([] { clientID = currentMessage.id; }),
				StateAction([] {send(Message((int)isServer,clientID,var1)); }),
			},{
				StateTransition("awaiting",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] {return currentMessage.command == var1; }),
						StateCondition([] {return currentMessage.id == clientID; }),
					}),
				StateTransition("start",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] {return currentMessage.command != var1 || currentMessage.id != clientID; }),
					}),
			}),
		State("awaiting",
			{
				
			},{
				StateTransition("base",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] {return currentMessage.command == var1; }),
						StateCondition([] {return currentMessage.id == clientID; }),
					}),
				StateTransition("sendReady",
					{
						StateCondition([] { return currentAction != 0; }),
						StateCondition([] { return newActionFlag; })
					}),
			}),
		State("sendReady",
			{
				StateAction([] {send(Message((int)isServer,0,go)); }),
			},{
				
				StateTransition("awaiting",
					{
					}),
			}),

		State("base",
			{
				StateAction([] { currentErrorState = currentState; })
			},{
				StateTransition("sendAction",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] { return currentMessage.command == 9; }),
						StateCondition([] { return currentMessage.direction != isServer; }),
						StateCondition([] { return currentMessage.id == clientID; })
					}),

			}),
		State("sendAction",
			{
				StateAction([] { send(Message((int)isServer, clientID, currentAction)); }),
				StateAction([] { currentAction=0; }),
				StateAction([] { newActionFlag = false; }),
				StateAction([] { currentActionPriority = 0; })
			},{
				StateTransition("base",
					{

					}),

			}),
	};
	
	// start decoder
	decoder::run(&process, false);

	// start thread
	stateMachine = std::thread(&dtmf::node::stateMachineThread);
}

// ...
void dtmf::node::initializeServer(void(*callback)(int payload, int id))
{
	if (isInitialized) {
		return;
	}

	isInitialized = true;
	isServer = true;

	//callbackFunction
	callbackFunction = callback;
	// state definitions for SERVER node

	errorTransition = StateTransition("start",
		{
			StateCondition([] { return false; })
		});

	states = {
		State("start",{
			
		},{
			StateTransition("newClient",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] { return currentMessage.id == 0; })
			}),
			StateTransition("informReady",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] { return currentMessage.command == go; }),
				StateCondition([] { return currentMessage.id != 0; })
			}),
			

		}),
		State("error",
			{
				StateAction([] { sync(); }),
				StateAction([] { send(Message((int)isServer, 0, 14)); }),
				StateAction([] { currentState=currentErrorState; })
			},{


			}),
		State("newClient",{
			StateAction([] { var1 = currentMessage.command; }),
			StateAction([] { send(Message((int)isServer, numClients+1, currentMessage.command)); })
		},{
			StateTransition("newClientReceived",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] {return currentMessage.command == var1; }),
				StateCondition([] {return currentMessage.id == numClients+1; }),
				}),
			StateTransition("start",{
				//StateCondition([] { return newMessageFlag; }),
				StateCondition([] {return currentMessage.command != var1 || currentMessage.id != numClients+1; }),
				})
		}),
		State("newClientReceived",{
			StateAction([] { send(Message((int)isServer, numClients + 1, var1)); }),
			StateAction([] { numClients++; }),
			StateAction([] { sync(); }),
		},{
			StateTransition("start",{

			}),

		}),
		State("informReady",{
			
			StateAction([] {sync(); send(Message((int)isServer, 0, go)); }),
			StateAction([] {sync(); send(Message((int)isServer, 0, go)); }),
			
		},{
			StateTransition("base",{

			}),

		}),
		State("base",{
		
			StateAction([] { sync(); }),
			StateAction([] { 
				errorTransition = StateTransition("error",
					{
						StateCondition([] { return checkTimeout(); })
					});
				}),
			StateAction([] { currentErrorState=currentState; })
		},{

			StateTransition("standardSend",{

				}),
		}),
		State("standardSend",{
		//StateAction([] { sync(); }),
		StateAction([] { send(Message((int)isServer, idCounter, 9)); })

		},{
			StateTransition("standardRecieve",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] { return currentMessage.id == idCounter; })
				}),

		}),
		State("standardRecieve",{ 
			StateAction([] {callbackFunction(currentMessage.command,currentMessage.id);  })
		},{
			StateTransition("standardNext",{

				}),

			}),
		State("standardNext",{
			StateAction([] { idCounter++; }),
			StateAction([] { if (idCounter > numClients) idCounter = 1; })
			},{
				StateTransition("standardSend",{

					}),

			}),

	};

	// start decoder
	decoder::run(&process, false);

	// start thread
	stateMachine = std::thread(&dtmf::node::stateMachineThread);
}