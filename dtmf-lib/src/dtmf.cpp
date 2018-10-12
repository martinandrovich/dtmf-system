#include <iostream>
#include "dtmf.h"
#include "signal/generator.h"
#include "Windows.h"

void dtmf::testFunction()
{
	std::cout << "Get ready for ear rape!" << std::endl;

	sf::Sound* testSound = new sf::Sound; // NEEDS GARBAGE CLEANING !!!!!!

	for (int i = 0; i <= 16; i++)
	{

		i = i % 16;

		int dur = 100;
		int brk = 20;

		testSound->setBuffer(*generator::generateDTMF(i, dur));
		testSound->play();
		Sleep(dur+brk);
	}	

}

void dtmf::testFunction2()
{	
	for (int i = 0; i <= 16; i++)
	{
		i = i % 16;
		generator::playback(i, 200);
	}	
}