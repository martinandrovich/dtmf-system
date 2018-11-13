#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
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

	~sampler2();

private:
	std::thread						worker;
	void thread();
	HWAVEIN     hWaveIn;
	WAVEHDR     WaveInHdr;
	//MMRESULT	result;
	WAVEFORMATEX pFormat;

	short int waveIn[NUMPTS];   
};

