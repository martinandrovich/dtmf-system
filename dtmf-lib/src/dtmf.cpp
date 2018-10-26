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

void keyPress(int key, int pause)
{

	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the key
	ip.ki.wVk = key; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	Sleep(pause);

	// Release the key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

void logPayload(uint toneID)
{
	
	std::cout << "[PAYLOAD]: " << toneID << std::endl;

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA
	
	int pause = 50;

	switch (toneID)
	{
	
	//Left
	case 0:
		keyPress(0x25, pause);
		break;

	// Right
	case 1:
		keyPress(0x27, pause);
		break;
	
	// Up
	case 2:
		keyPress(0x26, pause);
		break;
	
	// Down
	case 4:
		keyPress(0x28, pause);
		break;

	// Plant
	case 14:
		keyPress(0xBA, 25);
		break;
	}
}



void dtmf::testDecodeSample()
{
	decoder::init(&logPayload);
	decoder::run();
}