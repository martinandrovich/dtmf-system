#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>

#include "dtmf.h"

#include "signal/generator.h"
#include "signal/decoder.h"
#include "signal/sampler.h"

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

void logPayload(std::bitset<3> payload)
{
	std::cout << "[PAYLOAD] " << payload.to_string() << std::endl;
}

void dtmf::testDecodeSample()
{
	decoder::init(&logPayload);
	decoder::run();
}