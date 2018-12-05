#pragma once
#include <string>
#include <vector>

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{	

	//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

	namespace node
	{

		// Public Constructs
		enum actions { null, up, down, left, right, primary, secondary, menu , empty, input, proceed,chain,timed,exit,error,syncronize};
		//enum mode { pingMode, chainMode, timedMode};

		struct State;
		struct StateAction;
		struct StateTransition;
		struct StateCondition;

		struct State
		{
			State(std::string name, std::vector<StateAction> actions, std::vector<StateTransition> transitions) : name(name), actions(actions), transitions(transitions), isQuickState(false) {};
			State(std::string name, std::vector<StateAction> actions, std::vector<StateTransition> transitions, bool isQuickState) : name(name), actions(actions), transitions(transitions), isQuickState(isQuickState) {};
			std::string name;

			bool isQuickState;
			std::vector<StateAction> actions;
			std::vector<StateTransition> transitions;
		};

		struct StateAction
		{
			StateAction(void(*test)()) : function(test) {};
			void(*function)();
		};

		struct StateTransition
		{
			StateTransition() : targetName(""), targetId(-1), conditions() {};
			StateTransition(std::string targetName, std::vector<StateCondition> conditions) : targetName(targetName), targetId(-1), conditions(conditions) {};
			std::string targetName;
			int targetId;

			std::vector<StateCondition> conditions;
		};

		struct StateCondition
		{
			StateCondition(bool(*test)()) : result(test) {};
			bool(*result)();
			operator bool() const { return result(); }
		};

		struct Message
		{
			Message() : direction(-1), id(-1), address(-1), command(-1) {};
			Message(int command) : direction(-1), id(-1), address(-1), command(command) {};
			Message(int address, int command) : direction(address / 8), id(address & 7), address(address), command(command) {};
			Message(int direction, int id, int command) : direction(direction), id(id), address(direction * 8 + id), command(command) {};

			int command;
			int direction;
			int id;
			int address;
		};

		// Public Methods
		void initializeServer(void(*callback)(int payload, int id));
		void initializeClient(void(*callback)(int payload, int id));

		void sendPayload(int payload, int priority = 0);
		bool payloadReady();		

		// Public Members
		;

	}
}