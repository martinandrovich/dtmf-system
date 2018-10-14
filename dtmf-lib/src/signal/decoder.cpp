#include <thread>
#include <vector>
#include <queue>

#include "decoder.h"
#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	// Private Members
	state							status = state::unitialized;
	std::queue<std::vector<float>>	queue;
	std::thread						performer;

	void(*callback)() = nullptr;

	// Private Methods
	void perform();
	void add(std::vector<float> &samples);
	void decode(std::vector<float> &samples);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

void decoder::init(void(*callback)())
{
	decoder::callback = callback;
	performer = std::thread(&perform);
}

void decoder::run(void(*callback)())
{
	if (status == state::unitialized)
		init(callback);

	status == state::idle;
}

void decoder::end()
{
	// end

	performer.join();
}

void decoder::perform()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (status != state::idle)
			continue;
		
		if (!queue.empty())
			decode(queue.front());	// needs mutex management
	}
}

void decoder::add(std::vector<float> &samples)
{
	queue.push(samples);	// needs mutex management
}

void decoder::decode(std::vector<float> &samples)
{
	status == state::running;
	std::bitset<3> payload("010");

	// decode

	queue.pop();	// needs mutex management
	callback();
}