#include "sampler2.h"
#include <algorithm>
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
}

bool sampler2::start()
{
	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat,
		0L, 0L, WAVE_FORMAT_DIRECT);


	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;

	prepare();

	this->worker = new std::thread(&sampler2::thread, this);

	return true;
}

void sampler2::stop()
{
	worker->join();
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
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Insert a wave input buffer
	waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Commence sampling input
	waveInStart(hWaveIn);

	while (waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
	{

	}

	std::vector<short> samplesChunk(&waveIn[0], &waveIn[0] + NUMPTS);

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
