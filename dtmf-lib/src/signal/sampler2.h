#pragma once
#pragma comment(lib,"winmm.lib")
#include <windows.h>
#include <mmsystem.h>

#include <vector>
#include <functional>
#include <thread>
#include <atomic>

#include "constants.h"

class sampler2
{
public:

	// Constructor & Destructor
	sampler2(std::function<void(std::vector<short>)> callback , bool allowplayback = false );
	~sampler2();

	// Status Enum
	enum class state {
		unitialized,
		idle,
		sampling,
		processing
	};

	// Methods
	void					prepare();
	void					start();
	void					stop();

	std::vector<short>		sample();

	state					getStatus();

private:

	// Methods
	void					thread();

	// Members
	HWAVEIN					hWaveIn;
	WAVEHDR					WaveInHdr;
	MMRESULT				result;
	WAVEFORMATEX			pFormat;

	std::function<void(std::vector<short>)> callback;

	std::thread*			worker;
	state					status;
	bool					allowPlayback;
	std::atomic<bool>		running;

	short int				waveIn[NUMPTS];   
};

