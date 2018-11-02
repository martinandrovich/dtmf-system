#pragma once

class node
{
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////
public:

	// Constructor & Destructor
	node();
	virtual ~node();

	// Constructs
	struct Action
	{
		int clientID;
		enum actions {null, up, down, left, right, primary, secondary, menu };
	};

	struct Message
	{
		Message(int address, int command)			: direction(address / 8),	id(address & 7),	address(address),				command(command) {};
		Message(int direction, int id, int command) : direction(direction),		id(id),				address(direction * 8 + id),	command(command) {};

		int command;
		int direction;
		int id;
		int address;
	};

	// Methods
	void actionSend(Action::actions action);

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////
private:

	// Methods
	void send(int msg); // msg struct
	void process(int toneID);
	void recieved(Message msg);

	// Members
	void(*actionRecieved)(Action action);
};