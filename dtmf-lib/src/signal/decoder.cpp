#include <iostream>
#include <thread>
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
	std::queue<std::vector<float>>	queue;
	std::thread						worker;

	void(*callback)(std::bitset<3> payload);

	// Private Methods
	void thread();
	void add(std::vector<float> samples);
	void decode(std::vector<float> &samples);
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
		
		if (decoder::queue.empty())
			continue;

		decoder::decode(decoder::queue.front());	// needs mutex management
	}
}

// Add a (copy of) vector of samples to decoding queue
void decoder::add(std::vector<float> samples)
{	
	std::cout << "[QUEUE ADD] Adding to queue with [" << decoder::queue.size() << "] elements.\n";
	decoder::queue.push(samples);	// needs mutex management
}

// Decode an element from the queue
void decoder::decode(std::vector<float> &samples)
{
	decoder::status == state::working;
	std::bitset<3> fakePayload("010");

	// decode
	std::cout << "[DECODING] Decoding...\n";
	
	for (auto s : samples)
	{
		auto p = rand() % 5000 + 100;
		std::cout << s << " " << std::endl;
		Sleep(p);
	}

	decoder::queue.pop();
	decoder::callback(fakePayload);

	decoder::status == state::running;
}