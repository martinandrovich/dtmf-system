#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <Windows.h>

#include "decoder.h"
#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	// Private Members
	state							status = state::unitialized;
	std::queue<std::vector<short>>	queue;
	std::mutex						queueMutex;
	std::thread						worker;

	void(*callback)(std::bitset<3> payload);

	// Private Methods
	void thread();
	void add(std::vector<short> samples);
	void decode(std::vector<short> &samples);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the decoder
void decoder::init(void(*callback)(std::bitset<3> payload))
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

// Decode an element from the queue
void decoder::decode(std::vector<short> &samples)
{
	using namespace std::literals::chrono_literals;
	
	decoder::status = state::working;
	
	// decode
	std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n";
	std::bitset<3> fakePayload("010");

	std::this_thread::sleep_for(1000ms);

	decoder::callback(fakePayload);

	decoder::status = state::running;
}