#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#include "dtmf/node.h"
#include "signal/generator.h"
#include "signal/decoder.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	namespace node {

		// Private Members
		std::vector<State>	states;
		
		std::thread			stateMachine;
		std::mutex			messageMutex;

		bool				isInitialized = false;
		bool				isServer;
		bool				isQuickState = false;

		void(*callbackFunction)(int payload, int id);

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
		void process(uint toneID);
		void recieved(Message msg);
		
		void testCurrentState();
		int  getStateId(StateTransition& transition);
		bool testTransition(StateTransition& transition);

		void runStateActions();

		void checkAction();
		void checkMessage();
		void checkTimeOut();
		void checkTriggers();

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

// ...
void dtmf::node::process(uint toneID)
{
	messageMutex.lock();

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
	for (auto& transition : states[currentState].transitions)
	{
		if (testTransition(transition))
		{
			currentState = getStateId(transition);
			isQuickState = states[currentState].isQuickState;
			
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
void dtmf::node::checkTimeOut()
{
	;
}

// ...
void dtmf::node::checkTriggers()
{
	checkAction();
	checkMessage();
	checkTimeOut();
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

	// state definitions for CLIENT node
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
		State("connect",
			{
				StateAction([] { var1 = currentAction; }),
				StateAction([] { send(Message((int)isServer, 0, var1)); })
			},{
				StateTransition("setId",
					{
						StateCondition([] { return currentMessage.command == var1; }),
						StateCondition([] { return currentMessage.id != 0; })
					}),
				StateTransition("start",
					{
						StateCondition([] { return currentMessage.command != var1; })
					}),

			}),
		State("setId",
			{
				StateAction([] { clientID = currentMessage.id; }),
				StateAction([] {send(Message((int)isServer,clientID,var1)); }),
			},{
				StateTransition("base",
					{
						StateCondition([] {return currentMessage.command == var1; }),
						StateCondition([] {return currentMessage.id == clientID; }),
					}),
				StateTransition("start",
					{
						StateCondition([] {return currentMessage.command != var1 || currentMessage.id != clientID; }),
					}),
			}),
		State("base",
			{
			},{
				StateTransition("start",
					{
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

	// state definitions for SERVER node
	states = {
		State("start",{

		},{
			StateTransition("newClient",{
				StateCondition([] { return currentMessage.id == 0; })
			}),
			StateTransition("base",{
				StateCondition([] { return currentMessage.command == menu; }),
				StateCondition([] { return currentMessage.id != 0; })
			})

		}),
		State("newClient",{
			StateAction([] { var1 = currentMessage.command; }),
			StateAction([] { send(Message((int)isServer, numClients, currentMessage.command)); })
		},{
			StateTransition("newClientReceived",{
				StateCondition([] {return currentMessage.command == var1; }),
				StateCondition([] {return currentMessage.id == numClients; }),
				}),
			StateTransition("start",{
				StateCondition([] {return currentMessage.command != var1 || currentMessage.id != numClients; }),
				})
		}),
		State("newClientReceived",{
			StateAction([] { numClients++; }),
		},{
			StateTransition("start",{

			}),

		}),
		State("base",{
		},{

			StateTransition("standardSend",{

				}),
		}),
		State("standardSend",{
		StateAction([] { send(Message((int)isServer, idCounter, 9)); })

		},{
			StateTransition("standardRecieve",{
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