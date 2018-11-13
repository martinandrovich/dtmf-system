#include "sampler2.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "generator.h"

// Constructor
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

// Destructor
sampler2::~sampler2()
{
	stop();
}

// ...
bool sampler2::start()
{
	// prepare sampler
	this->prepare();

	// update variables
	this->status = state::sampling;
	sampling = true;	

	// start thread
	this->worker = new std::thread(&sampler2::thread, this);

	// return 
	return true;
}

// ...
void sampler2::stop()
{
	// update variables
	this->status	= state::idle;
	sampling		= false;
	
	// thread cleanup
	if (this->worker != nullptr)
	{
		worker->join();
		this->worker->~thread();
		delete this->worker;
	}

	// close handle
	waveInClose(hWaveIn);

	std::cout << "Sampler has been stopped.\n";
}

// ...
void sampler2::thread()
{
	while (sampling)
	{
		this->status = state::processing;

		// std::cout << "PROCESSING SAMPLES [" << NUMPTS << "] ...\n";

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

// ...
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

// ...
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

// ...
sampler2::state sampler2::getStatus()
{
	return status;
}