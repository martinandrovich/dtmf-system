#include <iostream>
#include "dtmf.h"
#include "signal/generator.h"
#include "Windows.h"

void dtmf::testFunction()
{
	std::cout << "Hello World!" << std::endl;

	if (!sf::SoundBufferRecorder::isAvailable())
	{
		std::cout << " error: audio capture is not available on this system" << std::endl;
		return;
	}

	sf::Sound* testSound = new sf::Sound; // NEEDS GARBAGE CLEANING !!!!!!

	testSound->setBuffer(*generator::generateDTMF('0'));
	testSound->play();

}