#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sampler
{
	// Private Members
	int					rate;
	int					interval;
	state				status		= state::unitialized;
	std::vector<float>	buffer;
	std::thread			worker;

	void(*callback)(std::vector<float> data);

	// Private Methods
	void thread();
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the sampler
void sampler::init(void(*callback)(std::vector<float> data), int interval, int rate)
{
	sampler::interval = interval;
	sampler::rate = rate;
	sampler::callback = callback;
	sampler::worker = std::thread(&sampler::thread);

	sampler::status = state::idle;
}

// Start the sampler
void sampler::run()
{
	if (sampler::status == state::unitialized)
		return;

	sampler::status = state::running;
}

// End the sampler
void sampler::end()
{
	if (sampler::status == state::unitialized)
		return;

	sampler::status = state::unitialized;
	sampler::worker.join();
}

// Thread function
void sampler::thread()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (sampler::status != state::running)
			continue;

		// Do some sampling
		std::cout << "[SAMPLING] Generating samples...\n";
		std::vector<float> fakeData = { 1, 2, 3 };

		std::this_thread::sleep_for(5000ms);

		sampler::callback(fakeData);		
	}
}

// Return the status variable
sampler::state sampler::getState()
{
	return sampler::status;
}