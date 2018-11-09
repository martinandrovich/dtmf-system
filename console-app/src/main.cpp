#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <dtmf/node.h>
#include <dtmf/toolbox.h>

int main()
{	
	
	// init
	std::cout << "Ear Rape Simulator 1.1\n\n";

	// test functions

	//std::cout << "Testing Decoder Keyboard Presses\n";
	//dtmf::toolbox::testDecoderKeyboardReciever();

	std::cout << "Testing Audio Conversion and Sample Plot\n";
	auto test = dtmf::toolbox::convertAudio("test.wav");
	dtmf::toolbox::plotSamples(test);

	// stall & exit
	std::cin.get();
	return 0;
}