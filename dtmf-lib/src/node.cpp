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

		bool				newMessageFlag, newPayloadFlag;
		int					clientID = -1;
		int					numClients = 0;
		int					idCounter = 1;

		int					currentState = 0;
		int					currentErrorState; //state to return to upon error signal
		bool				standardErrorHandling = true; //state to return to upon error signal
		int					currentPayload;
		int					currentPayloadPriority;
		Frame				currentMessage;

		mode				serverMode = chainMode;

		int					timeoutTimer;

		int					transmitions;
		int					failures;

		int					oldToneId;
		bool				hasRecievedDirID;
				
		int					var1 = 0, var2 = 0, var3 = 0, var4 = 0; //random access variables for state machine

		// Private Methods
		void send(Frame msg); // msg struct
		void sync();
		void process(uint toneID);
		void sleepForMessages(int n);
		void absorbMessage(int n);
		//void recieved(Message msg);
		
		void testCurrentState();
		int  getStateId(StateTransition& transition);
		bool testTransition(StateTransition& transition);

		void runStateActions();

		void checkPayload();
		void checkMessage();
		void checkIsTimeout();
		void checkTriggers();
		bool checkTimeout(int timeout = TIMEOUT);

		void stateMachineThread();
	}
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// ...
void dtmf::node::send(Frame msg)
{
	transmitions++;
	if (transmitions % 10 == 0) {
		std::cout <<"\n["<<transmitions<<"] Transmitions: " << transmitions << " Errors: " << failures << " Error rate: " << (float)failures / (float)transmitions << std::endl<<std::endl;
	}


	if (msg.address != -1) {

		std::vector<int> sequence = { msg.address, msg.command };
		generator::playbackSequence(sequence);
	}
	else {
		std::vector<int> sequence = { msg.command };
		generator::playbackSequence(sequence);
	}

	newPayloadFlag = false;
	currentPayload = null;
	currentPayloadPriority = 0;
}
void dtmf::node::sync()
{
	std::vector<int> sequence = { syncronize };
	hasRecievedDirID = false;
	generator::playbackSequence(sequence);

}
// ...
void dtmf::node::process(uint toneID)
{
	messageMutex.lock();
	node::timestamp = node::clock.now();
	std::cout << "Got tone [" << toneID << "]\n";
		if (toneID == syncronize) {
			hasRecievedDirID = false;
		}
		else if (hasRecievedDirID == false)
		{
			if (isQuickState)
			{
				currentMessage = Frame(toneID);
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
			currentMessage = Frame(oldToneId, toneID);
			hasRecievedDirID = false;
			newMessageFlag = true;
		}

	messageMutex.unlock();
}

//sleep For DURATION+PAUSE+LATENCYVARIANCE
void dtmf::node::sleepForMessages(int n) {
	
	std::this_thread::sleep_for(milliseconds((DURATION + PAUSE + LATENCYVARIANCE)*n));
		

}
// block and absorbs [n] full messages without proccessing
void dtmf::node::absorbMessage(int n) { 
	int c = 0;
	while (c < n){
		messageMutex.unlock();
		std::this_thread::sleep_for(milliseconds(1));
		messageMutex.lock();
		if (newMessageFlag) {
			currentMessage = Frame();
			newMessageFlag = false;
			c++;
		}

	}

}

// ...
void dtmf::node::sendPayload(int payload, int priority)
{
	messageMutex.lock();

		if (currentPayloadPriority <= priority) {
			currentPayload = payload;
			currentPayloadPriority = priority;
		}

		newPayloadFlag = true;

	messageMutex.unlock();
}

// ...
bool dtmf::node::payloadReady()
{
	return newPayloadFlag;
}




// ...
void dtmf::node::testCurrentState()
{
	if (standardErrorHandling) {

		if (testTransition(errorTransition))
		{
			currentState = getStateId(errorTransition);
			isQuickState = states[currentState].isQuickState;
			//timeoutTimer = 0;
			node::timestamp = node::clock.now();
			runStateActions();
			return;
		}
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
	for (auto& action : states[currentState].actions)
	{
		action.function();
	}
	currentMessage = Frame();
	newMessageFlag = false;
	testCurrentState();
}

// ...
void dtmf::node::checkPayload()
{
	messageMutex.lock();

		if (newPayloadFlag)
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
			currentMessage = Frame();
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
	checkPayload();
	checkMessage();
	checkIsTimeout();
}

// ...int timeout
bool dtmf::node::checkTimeout(int timeout)
{
	return ((int)static_cast<duration<double, std::milli>>(node::clock.now() - node::timestamp).count() > timeout);
}

// ...
void dtmf::node::stateMachineThread()
{
	while (true)
	{
		checkTriggers();

		std::this_thread::sleep_for(milliseconds(1));
		
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
			StateCondition([] { return currentMessage.command == error; }),
		});

	states = {
		State("start",
			{
			},{
				StateTransition("connect",
					{
						StateCondition([] { return currentPayload != null; }),
						StateCondition([] { return newPayloadFlag; })
					}),

			}),
		State("error",
			{
				StateAction([] { failures++; }),
				StateAction([] { currentState = currentErrorState; })
			},{
				

			}),
		State("connect",
			{
				StateAction([] { sync(); }),
				StateAction([] { var1 = currentPayload; }),
				StateAction([] { send(Frame((int)isServer, 0, var1)); })
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
				StateAction([] {send(Frame((int)isServer,clientID,var1)); }),
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
				StateTransition("start",
					{
						StateCondition([] { return checkTimeout(); })
					}),
			}),
		State("awaiting",
			{
				
			},{
				StateTransition("base",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] {return currentMessage.command == proceed; }),
						StateCondition([] {return currentMessage.id == 0; }),
					}),
				StateTransition("sendReady",
					{
						StateCondition([] { return currentPayload != 0; }),
						StateCondition([] { return newPayloadFlag; })
					}),
			}),
		State("sendReady",
			{
				StateAction([] {send(Frame((int)isServer,clientID,proceed)); }),
			},{
				
				StateTransition("awaiting",
					{
					}),
			}),

		State("base",
			{
				StateAction([] { currentErrorState = currentState; })
			},{
				StateTransition("chainBase",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] { return currentMessage.id == 0; }),
						StateCondition([] { return currentMessage.command == chain; }),
					}),

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
				StateAction([] { send(Frame((int)isServer, clientID, currentPayload)); }),
				StateAction([] { currentPayload=0; }),
				StateAction([] { newPayloadFlag = false; }),
				StateAction([] { currentPayloadPriority = 0; })
			},{
				StateTransition("base",
					{

					}),

			}),
		State("chainBase",
			{

			}, {
				StateTransition("chainSend",
					{
						StateCondition([] { return newMessageFlag; }),
						StateCondition([] { return currentMessage.command == proceed; }),
					}),

			},true),
		State("chainSend",
			{
				StateAction([] { absorbMessage(clientID-1); }),
				StateAction([] { send(Frame(currentPayload)); }),
			},{
				StateTransition("chainBase",
					{
						
					}),

			},true),
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
				StateCondition([] { return currentMessage.command == proceed; }),
				StateCondition([] { return currentMessage.id != 0; })
			}),
			

		}),
		State("error",
			{
				StateAction([] { failures++; }),
				StateAction([] { sync(); }),
				StateAction([] { send(Frame((int)isServer, 0, 14)); }),
				StateAction([] { currentState=currentErrorState; })
			},{


			}),
		State("newClient",{
			StateAction([] { var1 = currentMessage.command; }),
			StateAction([] { send(Frame((int)isServer, numClients+1, currentMessage.command)); })
		},{
			StateTransition("newClientReceived",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] {return currentMessage.command == var1; }),
				StateCondition([] {return currentMessage.id == numClients+1; }),
				}),
			StateTransition("start",{
				StateCondition([] { return newMessageFlag; }),
				StateCondition([] {return currentMessage.command != var1 || currentMessage.id != numClients+1; }),
				})
		}),
		State("newClientReceived",{
			StateAction([] { send(Frame((int)isServer, numClients + 1, var1)); }),
			StateAction([] { numClients++; }),
			StateAction([] { sync(); }),
		},{
			StateTransition("start",{

			}),

		}),
		State("informReady",{
			
			StateAction([] {sync(); send(Frame((int)isServer, 0, proceed)); }),
			StateAction([] {sync(); send(Frame((int)isServer, 0, proceed)); }),
			
		},{
			StateTransition("base",{

			}),

		}),
		State("base",{
		
			StateAction([] { sync(); }),
			StateAction([] { standardErrorHandling = true; }),
			StateAction([] { 
				errorTransition = StateTransition("error",
					{
						StateCondition([] { return checkTimeout(); })
					});
				}),
			StateAction([] { currentErrorState=currentState; })
		},{
			StateTransition("timeChainStart",{
					StateCondition([] { return serverMode==chainMode||serverMode==timedMode; }),

				}),

			StateTransition("standardSend",{

				}),
		}),
		State("standardSend",{
		//StateAction([] { sync(); }),
			StateAction([] { send(Frame((int)isServer, idCounter, 9)); })

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
		
			
		//==================================================================
		//Time Chain
		State("timeChainStart",{
			StateAction([] {sync(); send(Frame((int)isServer, 0, chain)); }),
			StateAction([] {idCounter=1;  })
			},{
				StateTransition("timeChainBase",{

				}),

			}),
		State("timeChainBase",{
				StateAction([] { send(Frame(proceed)); }),
				StateAction([] { idCounter = 1;  }),
				StateAction([] { standardErrorHandling=false; })
			},{
				StateTransition("base",{
				
					StateCondition([] { return serverMode==pingMode; }),
						
					
				}),
				StateTransition("timeChainAwaiting",{
					
				}),

			}),
			
		State("timeChainAwaiting",{
			},{
				StateTransition("timeChainRecieve",
				{
				
					StateCondition([] { return newMessageFlag; }),
				
				}),
				StateTransition("timeChainTimeout",
				{
					StateCondition([] { return checkTimeout(); })
				}),

			},true),
		State("timeChainRecieve",{
				StateAction([] {callbackFunction(currentMessage.command,idCounter);  })
			},{
				StateTransition("timeChainNext",{

				}),

			}),
		State("timeChainNext",{
			StateAction([] {idCounter++;  })
			},{
				StateTransition("timeChainBase",{
					StateCondition([] { return idCounter>numClients; })
				}),
				StateTransition("timeChainAwaiting",{

				}),

			}),
		State("timeChainTimeout",{
				StateAction([] { send(Frame(null)); })
			},{
				StateTransition("timeChainNext",{

				}),

			}),

	};

	// start decoder
	decoder::run(&process, false);

	// start thread
	stateMachine = std::thread(&dtmf::node::stateMachineThread);
}