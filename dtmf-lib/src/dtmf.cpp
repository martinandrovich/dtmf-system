#include <iostream>
#include <vector>
#include <Windows.h>

#include "dtmf.h"
#include "signal/generator.h"

void dtmf::testGenerator()
{	
	for (int i = 0; i <= 16; i++)
	{
		i = i % 16;
		generator::playback(i, 200);
	}	
}

void dtmf::testGeneratorSequence()
{
	std::vector<int> test = { 2, 0, 13, 7, 4 };
	generator::playbackSequence(test, 50);
}