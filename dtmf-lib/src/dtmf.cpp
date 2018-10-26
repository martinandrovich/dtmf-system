#include <iostream>
#include <vector>
#include <bitset>

#define WINVER 0x0500
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

void logPayload(uint toneID)
{
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the "A" key
	ip.ki.wVk = 0x41; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the "A" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));

	std::cout << "[PAYLOAD]: " << toneID << std::endl;
}

void dtmf::testDecodeSample()
{
	decoder::init(&logPayload);
	decoder::run();
}