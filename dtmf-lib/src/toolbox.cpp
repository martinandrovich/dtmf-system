#include "dtmf/toolbox.h"

#include "signal/generator.h"
#include "signal/decoder.h"

#define WINVER 0x0500

namespace dtmf
{

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace toolbox
{
	// Private Members
	;

	// Private Methods
	void logPayload(uint toneId);
	void executePayload(uint toneId);
	void pressKey(int key, int pause);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

///  Helper Methods ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Log a toneId
void toolbox::logPayload(uint toneId)
{
	std::cout << "[Payload]: " << toneId << "\n";
}

// Execute a keypress according to a toneId
void toolbox::executePayload(uint toneId)
{

	int keyMoveDuration			= 50;
	int keyPlantDuration		= 25;

	toolbox::logPayload(toneId);

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA

	// execute key press in accordance with toneID

	switch (toneId)
	{
		//Left
	case 0:
		toolbox::pressKey(0x25, keyMoveDuration);
		break;

		// Right
	case 1:
		toolbox::pressKey(0x27, keyMoveDuration);
		break;

		// Up
	case 2:
		toolbox::pressKey(0x26, keyMoveDuration);
		break;

		// Down
	case 4:
		toolbox::pressKey(0x28, keyMoveDuration);
		break;

		// Plant
	case 14:
		toolbox::pressKey(0xBA, keyPlantDuration);
		break;
	}
}

// Simulate global key press for a given duration
void toolbox::pressKey(int key, int duration)
{

	INPUT ip;

	// set up a generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// press the key
	ip.ki.wVk = key; // virtual-key code for the key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// wait
	Sleep(duration);

	// release the key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

///  Test Methods /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Playback all DTMF tones for 200ms
void toolbox::testGenerator()
{
	for (int i = 0; i <= 16; i++)
	{
		i = i % 16;
		generator::playback(i, 200);
	}
}

// Playback some DTMF tone sequence for 50ms each
void toolbox::testGeneratorSequence()
{
	std::vector<int> test = { 2, 0, 13, 7, 4 };
	generator::playbackSequence(test, 50);
}

// Initialize decoder and log the payload (toneId) of percieved DTMF tones
void toolbox::testDecoderLog()
{
	decoder::init(&dtmf::toolbox::logPayload);
	decoder::run();
}

// Initialize decoder and execute keypress according to the payload (toneId) of percieved DTMF tones
void toolbox::testDecoderKeyboard()
{
	decoder::init(&dtmf::toolbox::executePayload);
	decoder::run();
}

}