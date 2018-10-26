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
	void thread();
	void add(std::vector<short> samples);
	void decode(std::vector<short> &samples);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the decoder
void decoder::init(void(*callback)(uint toneID))
{
	decoder::callback = callback;
	decoder::worker = std::thread(&decoder::thread);

	sampler::init(&add);
	
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
	// end

	decoder::status = state::unitialized;
	decoder::worker.join();
}

// Thread function
void decoder::thread()
{
	using namespace std::literals::chrono_literals;

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

		auto samplesCopy = decoder::queue.front();				// make copy to faster unlock queue
		decoder::queue.pop();
		decoder::queueMutex.unlock();

		decoder::decode(samplesCopy);
		
	}
}

// Add a (copy of) vector of samples to decoding queue
void decoder::add(std::vector<short> samples)
{	
	decoder::queueMutex.lock();

	//std::cout << "[QUEUE] Adding to queue with [" << decoder::queue.size() << "] elements.\n";
	decoder::queue.push(samples);

	decoder::queueMutex.unlock();
}

// Convert DTMF frequencies to tone (id); return unsigned int
int decoder::convertDTMF(std::array<int, 2> positions)
{	
	int indexLow = positions[0];
	int indexHigh = positions[1];
	
	if (indexLow * indexHigh < 0)
		return -1;
	
	return (indexLow * 4 + indexHigh);
}

// Decode an element from the queue
void decoder::decode(std::vector<short> &samples)
{	
	decoder::status = state::working;
	
	// decode
	//std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n\n";
	
	auto data = processor::getDTMFPositions(samples);
	
	//auto data2 = processor::goertzelArray(samples);
	//processor::printGoertzelArray(data2);

	auto toneID = convertDTMF(data);

	if (toneID >= 0)
		callback(toneID);

	decoder::status = state::running;
}
