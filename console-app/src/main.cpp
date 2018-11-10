#pragma once
#include <iostream>
#include <string>
#include <vector>



void actionRecieved(int action, int id) {

}

#include <dtmf/node.h>
//#include <dtmf/toolbox.h>

int main()
{	
	




	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();

	// init
	std::cout << "Ear Rape Simulator 1.1\n\n";

	// test functions

	//std::cout << "Testing Decoder Keyboard Presses\n";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	std::vector<short> test = { 0, 2, 3, -4, 2, 1, -7 };
//	dtmf::toolbox::plotSamples(test);

	std::cout << "Testing Audio Conversion and Sample Plot\n";
	//auto test = dtmf::toolbox::convertAudio("test.wav");
	//dtmf::toolbox::plotSamples(test);
>>>>>>> master

	std::cin.get();
	return 0;


}



