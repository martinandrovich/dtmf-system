#pragma once
#include <vector>
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{

	// Public Constructs
	struct Action
	{
		int clientID;
		enum actions { null, up, down, left, right, primary, secondary, menu };
	};


	struct State;
	struct StateAction;
	struct StateTransition;
	struct StateCondition;
	
	struct State
	{
		State() {};
		State(std::string name, std::vector<StateAction> actions, std::vector<StateTransition*> transitions) : name(name), id(-1), actions(actions), transitions(transitions) {};
		std::string name;
		int id;
		std::vector<StateAction> actions;
		std::vector<StateTransition*> transitions;
	};
	struct StateAction
	{
		int a;
	};
	
	
	struct StateTransition
	{
		StateTransition(std::string targetName, std::vector<StateCondition*> conditions) : targetName(targetName), conditions(conditions) {};
		std::string targetName;
		int targetId;

		std::vector<StateCondition*> conditions;
	};
	
	struct StateCondition
	{
		StateCondition(bool(*test)()) : result(test) {};
		bool(*result)();
		explicit operator bool() { return result(); }
	};
	
	struct Message
	{
		Message(int address, int command) : direction(address / 8), id(address & 7), address(address), command(command) {};
		Message(int direction, int id, int command) : direction(direction), id(id), address(direction * 8 + id), command(command) {};

		int command;
		int direction;
		int id;
		int address;
	};

	// Public Methods
	void actionSend(Action::actions action);
	void initializeServer();
	void test();
}