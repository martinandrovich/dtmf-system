#pragma once
#include <iostream>
#include <dtmf.h>



void actionRecieved(dtmf::Action action) {

}

int main()
{	
	




	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();

	std::cin.get();
	return 0;

}



