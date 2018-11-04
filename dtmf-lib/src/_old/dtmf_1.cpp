#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>


#include <thread>

#define WINVER 0x0500

#include "dtmf.h"
#include "signal/generator.h"
#include "signal/decoder.h"


//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	// Private Methods
	void send(int msg); // msg struct
	void process(int toneID);
	void recieved(Message msg);

	int  testTransitions(const State &state);
	bool testConditions(const StateTransition &transition);
	void testCurrentState();

	// Private Members
	State* currentState;
	int clientID;

	std::vector<State> states;

	void(*actionRecieved)(Action action);

	int dir = 2;

}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

void dtmf::initializeServer()
{
	states = {

		State("Base", {
			StateAction()
		}, {
			StateTransition("Next", {
				StateCondition([] { return dir > 3; })
			})
		}),

		State("Next", {
			StateAction()
		}, {
			StateTransition("Base", {
				StateCondition([] { return dir < 3; })
			})
		}),

	};

	currentState = &states[0];
}

// test transitions of a state; return targetId if match (otherwise -1)
int dtmf::testTransitions(const State &state)
{
	for (const auto& transition : state.transitions)
	{
		if (dtmf::testConditions(transition))
		{
			return transition.targetId;
		}
	}

	return -1;
}

// test conditions of a transition; return true if match
bool dtmf::testConditions(const StateTransition &transition)
{
	for (const auto& condition : transition.conditions)
	{
		if (condition.result())
		{
			return true;
		}
	}

	return false;
}

// test current state (transitions & conditions) in one method; change state if match
void dtmf::testCurrentState()
{
	for (const auto& transition : currentState->transitions)
	{
		for (const auto& condition : transition.conditions)
		{
			if (condition.result())
			{
				currentState = &states[transition.targetId];
			}
		}
	}
}

// test
void dtmf::test()
{
	dir = 2;
	dtmf::testTransitions(*currentState); // no match

	dir = 4;
	dtmf::testTransitions(*currentState); // match
}}