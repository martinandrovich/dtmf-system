#pragma once
#include <iostream>
#include <dtmf.h>



void actionRecieved(int action, int id) {

}

int main()
{	
	




	dtmf::initializeServer(&actionRecieved);




	//dtmf::testDecodeKeyboard();

	std::cin.get();
	return 0;

}



