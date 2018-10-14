#include <thread>
#include <vector>
#include <queue>

#include "decoder.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	
	// Private Members
	state							status = state::unitialized;
	std::queue<std::vector<int>>	queue;
	std::thread						performer;

	void(*callback)() = nullptr;

	// Private Methods
	void init(void(*callback)());
	void perform();
	void decode(std::vector<int> &samples);
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
}

void decoder::perform()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (status != state::idle)
			continue;
		
		if (!queue.empty())
			decode(queue.front());
	}
}

void decoder::decode(std::vector<int> &samples)
{
	status == state::running;

	// decode

	callback();
}