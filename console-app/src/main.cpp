#pragma once
#include <iostream>
#include <node.h>



void actionRecieved(int action, int id) {

}
<<<<<<< HEAD

#include <dtmf/node.h>
#include <dtmf/toolbox.h>
=======
>>>>>>> f1ae74b4d136d5edb6245a60dc70985fba6a99bf

int main()
{	
	




	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();
<<<<<<< HEAD

	// init
	std::cout << "Ear Rape Simulator 1.1\n";

	// test functions
	std::cout << "Testing Decoder Keyboard Presses";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	std::vector<short> test = { 0, 2, 3, -4, 2, 1, -7 };
	dtmf::toolbox::plotSamples(test);

=======
>>>>>>> f1ae74b4d136d5edb6245a60dc70985fba6a99bf

	std::cin.get();
	return 0;
<<<<<<< HEAD

=======
>>>>>>> f1ae74b4d136d5edb6245a60dc70985fba6a99bf

}



