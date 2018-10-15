#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

#include "constants.h"
#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sampler
{
	// Private Members
	int					rate		= SAMPLE_RATE;
	int					interval	= SAMPLE_INTERVAL;
	state				status		= state::unitialized;
	std::vector<float>	buffer;
	std::thread			worker;

	void(*callback)(std::vector<float> data);

	// Private Methods
	void thread();
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the sampler
void sampler::init(void(*callback)(std::vector<float> data))
{
	sampler::callback = callback;
	worker = std::thread(&sampler::thread);

	status = state::idle;
}

// Start the sampler
void sampler::run()
{
	if (status == state::unitialized)
		return;

	status = state::running;
}

// End the sampler (thread still active)
void sampler::end()
{
	if (status == state::unitialized)
		return;

	status = state::unitialized;
	worker.join();
}

// Thread function
void sampler::thread()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (status != state::idle)
			continue;

		// Do some sampling
		std::cout << "Sampling...\n";
		std::this_thread::sleep_for(1000ms);
		std::vector<float> fakeData = { 1.2, 20.f, 10.f, 420.0, 0.01 };
		
		callback(fakeData);
	}
}

// Return the status variable
sampler::state sampler::getState()
{
	return sampler::status;
}