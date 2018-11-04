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
		StateTransition(std::string targetName, std::vector<StateCondition> conditions) : targetName(targetName), targetId(-1), conditions(conditions) {};
		std::string targetName;
		int targetId;

		std::vector<StateCondition> conditions;
	};
	
	struct StateCondition
	{
		StateCondition(bool(*test)()) : result(test) {};
		bool(*result)();
		explicit operator bool() { return result(); }
	};
	
	struct Message
	{
		Message() : direction(-1), id(-1), address(-1), command(-1) {};
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