#include <iostream>
#include "dtmf.h"
#include "signal/generator.h"
#include "Windows.h"

void dtmf::testFunction()
{
	std::cout << "Get ready for ear rape!" << std::endl;

	if (!sf::SoundBufferRecorder::isAvailable())
	{
		std::cout << " error: audio capture is not available on this system" << std::endl;
		return;
	}

	sf::Sound* testSound = new sf::Sound; // NEEDS GARBAGE CLEANING !!!!!!

	for (int i = 0; i <= 16; i++)
	{

		i = i % 16;

		int dir = 100;
		int brk = 20;

		testSound->setBuffer(*generator::generateDTMF(i, dir));
		testSound->play();
		Sleep(dir+brk);
	}	

}