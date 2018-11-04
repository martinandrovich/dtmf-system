#pragma once
#include <vector>
#include <mutex>
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{

	// Public Constructs
	struct Action
	{
		int clientID;
		enum actions { null, up, down, left, right, primary, secondary, menu };
	};
	enum class indexValues { C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15, clientID, currentState, var1, var2, var3, var4, action, direction, id, data, randomValue };

	struct State;
	struct StateAction;
	struct StateTransition;
	struct StateCondition;
	
	struct State
	{
		State(std::string name, std::vector<StateAction> actions, std::vector<StateTransition> transitions) : name(name), id(-1), actions(actions), transitions(transitions) {};
		std::string name;
		int id;
		std::vector<StateAction> actions;
		std::vector<StateTransition> transitions;
	};
	struct StateAction
	{
		int a;
	};
	
	
	struct StateTransition
	{
		StateTransition(std::string targetName, std::vector<StateCondition> conditions) : targetName(targetName), targetId(-1), conditions(conditions) {};
		std::string targetName;
		int targetId;

		std::vector<StateCondition> conditions;
	};
	
	struct StateCondition
	{
		StateCondition(int subject, int test) : subject(subject), test(test) {};
		int subject;
		int test;
	};
	
	struct Message
	{
		Message() : direction(0), id(0), address(0), command(0) {};
		Message(int address, int command) : direction(address / 8), id(address & 7), address(address), command(command) {};
		Message(int direction, int id, int command) : direction(direction), id(id), address(direction * 8 + id), command(command) {};

		int command;
		int direction;
		int id;
		int address;
	};




	// Public Methods
	void actionSend(Action::actions action);
	void initializeServer(void(*callback)(Action action));
	


	// Public Members

	




}