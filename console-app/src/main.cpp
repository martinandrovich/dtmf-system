#pragma once
#include <iostream>
#include <dtmf.h>



void actionRecieved(dtmf::Action action) {

}

int main()
{	
	
	std::cout << "Ear Rape Simulator 1.0\n";

	std::cout << "Testing Decoder w/ keypresses\n";



	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();

	std::cin.get();
	return 0;

}



