#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>

#define WINVER 0x0500

#include "dtmf/dtmf.h"

#include "signal/generator.h"
#include "signal/decoder.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	// Private Members
	;

	// Private Methods
	void logPayload(uint toneID);
	void executePayload(uint toneID);
	void keyPress(int key, int pause);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Playback all DTMF tones for 200ms
void dtmf::testGenerator()
{	
	for (int i = 0; i <= 16; i++)
	{
		i = i % 16;
		generator::playback(i, 200);
	}
}

// Playback some DTMF tone sequence for 50ms each
void dtmf::testGeneratorSequence()
{
	std::vector<int> test = { 2, 0, 13, 7, 4 };
	generator::playbackSequence(test, 50);
}

// ...
void dtmf::testDecodeLog()
{
	decoder::init(&dtmf::logPayload);
	decoder::run();
}

// ...
void dtmf::testDecodeKeyboard()
{
	decoder::init(&dtmf::executePayload);
	decoder::run();
}

// ...
void dtmf::logPayload(uint toneID)
{
	std::cout << "[PAYLOAD]: " << toneID << std::endl;
}

// ...
void dtmf::executePayload(uint toneID)
{

	int keyMoveDuration = 50;
	int keyPlantDuration = 25;

	logPayload(toneID);

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA

	// execute key press in accordance with toneID

	switch (toneID)
	{
	//Left
	case 0:
		keyPress(0x25, keyMoveDuration);
		break;

	// Right
	case 1:
		keyPress(0x27, keyMoveDuration);
		break;

	// Up
	case 2:
		keyPress(0x26, keyMoveDuration);
		break;

	// Down
	case 4:
		keyPress(0x28, keyMoveDuration);
		break;

	// Plant
	case 14:
		keyPress(0xBA, keyPlantDuration);
		break;
	}
}

// ...
void dtmf::keyPress(int key, int pause)
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