#include <iostream>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

int main()
{	
	
	// init
	std::cout << "Ear Rape Simulator 1.1\n";

	// test functions
	std::cout << "Testing Decoder Keyboard Presses";
	dtmf::toolbox::testDecoderKeyboard();
	
	dtmf::cake test(10);

	// stall & exit
	std::cin.get();
	return 0;
}