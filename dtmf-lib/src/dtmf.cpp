#include <iostream>
#include <vector>
#include <Windows.h>

#include "dtmf.h"
#include "signal/generator.h"

void dtmf::testFunction()
{
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

void dtmf::testFunction3()
{
	std::vector<int> test = { 2, 0, 13, 7, 4 };
	generator::playbackSequence(test, 50);
}