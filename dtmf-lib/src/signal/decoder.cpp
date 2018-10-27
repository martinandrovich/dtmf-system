#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <array>
#include <stdlib.h>
#include <Windows.h>

#include "decoder.h"
#include "sampler.h"
#include "processor.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	// Private Members
	state							status = state::unitialized;
	std::queue<std::vector<short>>	queue;
	std::mutex						queueMutex;
	std::thread						worker;

	void(*callback)(uint tone);

	// Private Methods
	void					thread();
	void					decode(std::vector<short> &samples);
	void					appendQueue(std::vector<short> samples);
	std::array<int, 2>		extractIndexes(std::array<float, 8> &goertzelArray);
	int						extractToneID(std::array<int, 2> indexes);
	
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the decoder
void decoder::init(void(*callback)(uint toneID))
{
	decoder::callback = callback;
	decoder::worker = std::thread(&decoder::thread);

	sampler::init(&appendQueue);
	
	decoder::status = state::idle;
}

// Start the decoder
void decoder::run()
{
	if (decoder::status == state::unitialized)
		return;

	sampler::run();
	decoder::status = state::running;
}

// End the decoder
void decoder::end()
{
	// stop the sampler
	sampler::end();

	// update state + join thread
	decoder::status = state::unitialized;
	decoder::worker.join();
}

// Thread function
void decoder::thread()
{
	while (true)
	{		
		if (decoder::status != state::running)
			continue;

		decoder::queueMutex.lock();

			if (decoder::queue.empty())								// better done with a condition variable?
			{
				decoder::queueMutex.unlock();
				continue;
			}

			auto samplesCopy = decoder::queue.front();				// make copy so that queue isn't blocked while decoding
			decoder::queue.pop();

		decoder::queueMutex.unlock();
		
		// decode the copied samples
		decoder::decode(samplesCopy);
	}
}

// Add a (copy of) vector of samples to decoding queue
void decoder::appendQueue(std::vector<short> samples)
{	
	decoder::queueMutex.lock();

	//std::cout << "[QUEUE] Adding to queue with [" << decoder::queue.size() << "] elements.\n";
	decoder::queue.push(samples);

	decoder::queueMutex.unlock();
}

// Convert goertzelArray to indexes (row & column) of two most prominent frequencies; return array[2]
std::array<int, 2> decoder::extractIndexes(std::array<float, 8> &goertzelArray)
{
	float magnitudeLowMax		= 0;
	float magnitudeHighMax		= 0;

	std::array<int, 2> indexes	= { -1, -1 };

	// Iterate through array of goertzel magnitudes
	for (uint i = 0; i < goertzelArray.size(); i++)
	{
		// define magnitude & threshold for current frequency index (i)
		auto magnitude = goertzelArray[i];
		auto threshold = freqThresholds[i];

		// low frequencies
		if (i < 4 && magnitude > threshold && magnitude > magnitudeLowMax)
		{
			magnitudeLowMax = magnitude;
			indexes[0] = i;
		}

		// high frequencies
		if (i >= 4 && magnitude > threshold && magnitude > magnitudeHighMax)
		{
			magnitudeHighMax = magnitude;
			indexes[1] = (i - 4);
		}
	}

	// return array of indexes
	return indexes;
}

// Convert DTMF indexes (row & column) to toneID (0-15); return int
int decoder::extractToneID(std::array<int, 2> indexes)
{
	int indexLow	= indexes[0];
	int indexHigh	= indexes[1];

	if ((indexLow * indexHigh) < 0)
		return -1;

	return (indexLow * 4 + indexHigh);
}

// Decode an chunk of samples from the queue
void decoder::decode(std::vector<short> &samples)
{	
	decoder::status = state::working;
	
	// decode
	//std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n\n";

	// combine 5 latest sample chunks
	;

	// compile goertzelArray for all DTMF frequencies
	auto goertzelArray = processor::goertzelArray(samples);

	// check if deltaAmplitude is falling
	;

	// extract indexes (row & column) of most prominent frequencies
	auto indexes = decoder::extractIndexes(goertzelArray);

	// convert indexes to DTMF toneID
	auto toneID = decoder::extractToneID(indexes);

	// callback
	if (toneID >= 0)
		callback(toneID);

	decoder::status = state::running;
}
