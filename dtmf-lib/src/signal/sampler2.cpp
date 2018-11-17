#include <windows.h>
#include <mmsystem.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include "generator.h"

#include "sampler2.h"

// Constructor
sampler2::sampler2(std::function<void(std::vector<short>)> callback, bool allowPlayback )
	: callback(callback), allowPlayback(allowPlayback), running(false)
{
	// sampling properties
	pFormat.wFormatTag		= WAVE_FORMAT_PCM;			// simple, uncompressed format
	pFormat.nChannels		= 1;						//  1=mono, 2=stereo
	pFormat.nSamplesPerSec	= SAMPLE_RATE;				// 44100
	pFormat.wBitsPerSample	= 16;						//  16 for high quality, 8 for telephone-grade
	pFormat.nAvgBytesPerSec = pFormat.nSamplesPerSec * pFormat.nChannels*(pFormat.wBitsPerSample/8);
	pFormat.nBlockAlign		= 2;						// = n.Channels * wBitsPerSample/8
	pFormat.cbSize			= 0;

	// open wave hander
	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat,	0L, 0L, WAVE_FORMAT_DIRECT);

	// wave header properties
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;

	// log
	std::cout << "\nInitialized sampler2 with:\n";
	std::cout << "SAMPLE RATE:\t\t"			<< SAMPLE_RATE << " Hz \n";
	std::cout << "SAMPLE INTERVAL:\t"		<< SAMPLE_INTERVAL << " ms \n";
	std::cout << "BG PLAYBACK:\t\t"			<< (allowPlayback ? "true" : "false") << "\n\n";
}

// Destructor
sampler2::~sampler2()
{
	if (this->running)
	{
		this->stop();
	}	
}

// Start the sampler
void sampler2::start()
{	
	// prepare sampler
	this->prepare();

	// update variables
	this->status	= state::sampling;
	this->running	= true;	

	// start thread
	this->worker = new std::thread(&sampler2::thread, this);
}

// Stop sampler; waits for current iteration to finish
void sampler2::stop()
{
	// update variables
	this->status	= state::idle;
	this->running	= false;
	
	// thread cleanup
	if (this->worker != nullptr)
	{
		worker->join();
		this->worker->~thread();
		delete this->worker;
	}

	// unprepare header
	waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// close handle
	waveInClose(hWaveIn);

	// done
	std::cout << "Sampler has been stopped.\n";
}

// The callback sampler thread; initiated by .start()
void sampler2::thread()
{
	using namespace std::chrono;

	while (running)
	{
		// continue sampling
		this->status = state::sampling;

		// timing
		//high_resolution_clock				clock;
		//time_point<high_resolution_clock>	timeStart;

		// std::cout << "PROCESSING SAMPLES [" << NUMPTS << "] ...\n";

		// create vector of samples
		std::vector<short> samples = sample();

		// process data
		this->status = state::processing;

		// return silent (zero) array if playback disallowed and generator is playing
		if (!this->allowPlayback && generator::getState() == generator::state::playing)
		{
			std::fill(samples.begin(), samples.end(), 0);
		}

		//timeStart = clock.now();

		// callback with copy of samples chunk
		this->callback(samples);

		//std::cout << "Callback took: " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << " ms\n";
	}
}

// Prepare the sampler; block thread until a sample chunk has actual audio data
void sampler2::prepare()
{
	using namespace std::chrono;
	
	// variables
	int counter = 0;
	std::vector<short> samplesChunk(NUMPTS);
	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart = clock.now();

	while (true)
	{
		// update counter
		counter++;

		// get a chunk of samples
		samplesChunk = sample();

		// check whether any absolute value of chunk is greatear than 2; break if true.
		if (std::any_of(samplesChunk.begin(), samplesChunk.end(), [&](short sample) {return abs(sample) > 2; }))
		{
			std::cout << "Ready after: " << counter << " chunk [ " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << " ms]\n";
			break;
		}
	}
}

// Perform a single sampling; return sample chunk (array)
std::vector<short> sampler2::sample()
{
	using namespace std::chrono;

	// timing variables
	//high_resolution_clock				clock;
	//double dur;
	//time_point<high_resolution_clock>	timeStart = clock.now();

	// prepare header
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// insert a wave input buffer
	waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// commence sampling input
	waveInStart(hWaveIn);
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	/*while (waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
	{

	}*/
	
	// sleep for the duration of a sample interval
	std::this_thread::sleep_for(milliseconds(SAMPLE_INTERVAL));

	/*

	Can be optimized using a ringbuffer and pointers, such that the waveIn pointer is changed
	and header preparations only need to be performed once, while a background thread cleans up.
	
	Although this should have a separate method, e.g. sample_many(int num_chunks);
	
	*/

	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// create vector
	std::vector<short> samplesChunk(&waveIn[0], &waveIn[0] + NUMPTS);
	//dur = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

	// return vector
	return samplesChunk;
}

// Return sampler status
sampler2::state sampler2::getStatus()
{
	return status;
}