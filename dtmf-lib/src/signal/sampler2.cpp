#include "sampler2.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "generator.h"

sampler2::sampler2(std::function<void(std::vector<short>)> callback, bool allowplayback )
{
	pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels = 1;                    //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = SAMPLE_RATE;      // 44100
	pFormat.wBitsPerSample = 16;              //  16 for high quality, 8 for telephone-grade
	pFormat.nAvgBytesPerSec = pFormat.nSamplesPerSec * pFormat.nChannels*(pFormat.wBitsPerSample/8);
	pFormat.nBlockAlign = 2;                  // = n.Channels * wBitsPerSample/8
	pFormat.cbSize = 0;

	this->callback = callback;
	allowPlayback = allowplayback;

	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat,
		0L, 0L, WAVE_FORMAT_DIRECT);


	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
}

bool sampler2::start()
{
	

	prepare();

	this->worker = new std::thread(&sampler2::thread, this);

	return true;
}

void sampler2::stop()
{
	if (this->worker != nullptr)
	{
		this->worker->join();
		delete this->worker;
	}

	waveInClose(hWaveIn);
}

void sampler2::thread()
{
	while (true)
	{
		this->status = state::processing;

		std::cout << "PROCESSING SAMPLES [" << NUMPTS<< "] ...\n";

		// create vector of samples
		
		std::vector<short> samplesCopy = sample();

		// return silent (zero) array if generator is playing
		// NOT THREAD SAFE !#!#!#!#!#!#!#!#!#!#!						!#!#!#!#!#!#!#!#!#!#!!#!
		if (!this->allowPlayback && generator::getState() == generator::state::playing)
		{
			std::fill(samplesCopy.begin(), samplesCopy.end(), 0);
		}

		// callback with copy of samples chunk
		this->callback(samplesCopy);

		// continue sampling
		this->status = state::sampling;
	}
}

void sampler2::prepare()
{
	int counter = 0;
	std::vector<short> samplesChunk(NUMPTS);
	while (true)
	{
		counter++;

		samplesChunk = sample();
		if (std::any_of(samplesChunk.begin(), samplesChunk.end(), [](short sample) {return abs(sample) > 2; }))
		{
			std::cout << "Ready after: " << counter << " chunks" << std::endl;
			break;
		}
	}
}

std::vector<short> sampler2::sample()
{
	using namespace std::chrono;

	high_resolution_clock				clock;
	double dur;
	time_point<high_resolution_clock>	timeStart = clock.now();

	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// Insert a wave input buffer
	waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// Commence sampling input
	waveInStart(hWaveIn);
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	/*while (waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
	{

	}*/
	
	// sleep as needed
	std::this_thread::sleep_for(milliseconds(SAMPLE_INTERVAL));

	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	std::vector<short> samplesChunk(&waveIn[0], &waveIn[0] + NUMPTS);
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	//std::vector<short> samplesChunk;
	return samplesChunk;
}

sampler2::state sampler2::getStatus()
{
	return status;
}

sampler2::~sampler2()
{
	stop();
}
