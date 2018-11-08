#include <iostream>
<<<<<<< HEAD
#include <dtmf.h>
#define LOG(x) std::cout << x << std::endl;
=======

#include <dtmf/node.h>
#include <dtmf/toolbox.h>
>>>>>>> master

void init() {
	char ID;
	std::cout << "Er jeg server eller klient? [SPACE]/g \n";
	char sok = std::getchar();
	if (sok == ' ') {
		std::cout << "Server valgt \n";
		ID = 'S';
	}
	else {
		std::cout << "Klient valgt \n";
		ID = 'K';
	}

	switch (ID)
	{
	case 'S': {
		char asd;
		std::cout << "Vil du tilkoble klienter, y/n? \n";
			std::cin >> asd;
		if (asd == 'y') {
			dtmf::testGenerator();
		}
	}
	case 'K': {
		while (true)
			return;
	}
	}
}
int main()
{	
<<<<<<< HEAD

	init();
=======
	
	// init
	std::cout << "Ear Rape Simulator 1.1\n";

	// test functions
	std::cout << "Testing Decoder Keyboard Presses";
	dtmf::toolbox::testDecoderKeyboard();
>>>>>>> master
	
	dtmf::cake test(10);

	// stall & exit
	std::cin.get();
	return 0;
}