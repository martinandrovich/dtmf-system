#include <iostream>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

int main()
{	
	
	// init
	std::cout << "Ear Rape Simulator 1.1\n";

	// test functions
	std::cout << "Testing Decoder Keyboard Presses";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	std::vector<short> test = { 0, 2, 3, -4, 2, 1, -7 };
	dtmf::toolbox::plotSamples(test);

	// stall & exit
	std::cin.get();
	return 0;
}