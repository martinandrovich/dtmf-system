#include <iostream>
#include <string>
#include <stdlib.h>
#include <dtmf/node.h>
#include <dtmf/toolbox.h>
#include <chrono>
#include <thread>
//needed for keystroke monitoring
#include <windows.h>

#define LOG(x) std::cout << x << std::endl;
void assignClients(int am)
{
	int amount = am;
	for (int i = 0; i < amount; i++)
	{
		std::cout << "\n" << "Tilkobler spiller " << i << " ";
		for (int j = 0; j < 3; j++)
		{

			std::cout << ".";
			std::this_thread::sleep_for(std::chrono::milliseconds(600));
		}

	}
}
int listenForKey()
{
	int key=0;
	if (GetKeyState('S') < 0) 
		LOG("s down")
	if (GetKeyState(VK_LEFT) < 0)
		key = 1;
	if (GetKeyState(VK_UP) < 0)
		key = 2;
	if (GetKeyState(VK_DOWN) < 0)
		key = 3;
	if (GetKeyState(VK_RIGHT) < 0)
		key = 4;
	if (GetKeyState(VK_SPACE) < 0)
		key=5;
	if (GetKeyState(VK_CONTROL) < 0)
		key = 6;
	if (GetKeyState(VK_ESCAPE) < 0)
		key = 7;

	return key;
}
//Main method for klienter, bør måske implementeres som Klasse (objekt). //Kræver windows.h
void clientWork()
{
	while (true)
	{
		//Implementering af prioritetsliste: esc>actions>movement
		int payload = listenForKey();
		int priority;
		if (payload < 5)
			priority = 0;
		else if (payload < 7)
			priority = 1;
		else priority = 2;
		//node::sendPayload(payload, prioritet)
			if (keydown != 0)
				LOG(keydown);

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

void init() {
	std::string input;
	int nInput;
	char ID;
	std::cout << "Er jeg server eller klient? [SPACE]/g/w \n";
	std::getline(std::cin, input);
	if (input == " ") {
		std::cout << "Server valgt \n";
		ID = 'S';
		//LOG("Hvor mange klienter ønsker du at tilkoble");
		//std::cin >> nInput;
		//assignClients(nInput);
		void serverWork();
	}
	else {
		std::cout << "Klient valgt \n";
		ID = 'K';
		clientWork();
	}
}
void serverWork()
	{
		ShellExecute(0, 0, "https://www.playing-with-fire-game.com/", 0, 0, SW_SHOW);
	}





int main()
{	
	// init
	init();
	// test functions
	//std::cout << "Testing Decoder Keyboard Presses";
	//dtmf::toolbox::testDecoderKeyboard();
	
	// stall & exit
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();

	return 0;
}