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
	int key;
	if (GetKeyState(VK_LEFT) == true)
		key = 1;
	if (GetKeyState(VK_UP) == true)
		key = 2;
	if (GetKeyState(VK_DOWN) == true)
		key = 3;
	if (GetKeyState(VK_RIGHT) == true)
		key = 4;
	if (GetKeyState(VK_SPACE) == true)
		key = 5;
	if (GetKeyState(VK_LEFT) == true)
		key = 6;

	return key;
}
//Main method for klienter, bør måske implementeres som Klasse (objekt). //Kræver windows.h
void clientWork()
{
	while (true)
	{
		LOG(listenForKey())
	
	}
}

void init() {
	std::string input;
	int nInput;
	char ID;
	std::cout << "Er jeg server eller klient? [SPACE]/g \n";
	std::getline(std::cin, input);
	if (input == " ") {
		std::cout << "Server valgt \n";
		ID = 'S';
		LOG("Hvor mange klienter ønsker du at tilkoble");
		std::cin >> nInput;
		assignClients(nInput);
	
	}
	else {
		std::cout << "Klient valgt \n";
		ID = 'K';
		clientWork();
	}

	switch (ID)
	{
	case 'S': {
	
	}
	case 'K': {
		while (true)
			return;
	}
	}
}



int main()
{	
	// init
	init();
	// test functions
	//std::cout << "Testing Decoder Keyboard Presses";
	//dtmf::toolbox::testDecoderKeyboard();
	
	LOG("yooo")
	// stall & exit
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();

	return 0;
}