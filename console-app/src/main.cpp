#pragma once
#include <iostream>



void actionRecieved(int action, int id) {

}

#include <dtmf/node.h>
//#include <dtmf/toolbox.h>

int main()
{	
	




	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();

	// init
	std::cout << "Ear Rape Simulator 1.1\n";

	// test functions
	std::cout << "Testing Decoder Keyboard Presses";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	std::vector<short> test = { 0, 2, 3, -4, 2, 1, -7 };
//	dtmf::toolbox::plotSamples(test);


	std::cin.get();
	return 0;


}



