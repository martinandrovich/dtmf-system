#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include "constants.h"

class sampler2
{
public:

	sampler2(std::function<void(std::vector<short>)> callback , bool allowplayback = false );

	bool start();
	void stop();

	void prepare();
	std::vector<short> sample();

	enum class state {
		unitialized,
		idle,
		sampling,
		processing
	};

	state	getStatus();

	~sampler2();

private:
	std::thread*			worker;
	void thread();
	HWAVEIN     hWaveIn;
	WAVEHDR     WaveInHdr;
	//MMRESULT	result;
	WAVEFORMATEX pFormat;

	std::function<void(std::vector<short>)> callback;

	state				status;
	bool				allowPlayback;

	short int waveIn[NUMPTS];   
};

